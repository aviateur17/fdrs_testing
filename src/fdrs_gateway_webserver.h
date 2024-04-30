#ifdef ESP32
// #include <FS.h>
// #include <SPIFFS.h>
// #include <ESPmDNS.h>
// #include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
// #include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
// #include <ESP8266mDNS.h>
#endif
#include <ESPAsyncWebServer.h>
// #include <SPIFFSEditor.h>

#include "fdrs_gateway_webpage.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
// AsyncEventSource events("/events");

// Simple function to send information to the web clients
void sendJson(AsyncWebSocketClient * client, String l_type, String l_value) {
    String jsonString = "";                           // create a JSON string for sending data to the client
    JsonDocument doc;                      // create JSON container
    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    object["type"] = l_type;                          // write data into the JSON object -> I used "type" to identify if LED_selected or LED_intensity is sent and "value" for the actual value
    object["value"] = l_value;
    serializeJson(doc, jsonString);                // convert JSON object to string
    client->text(jsonString);               // send JSON string to all clients
}
void sendJson(String l_type, String l_value) {
    String jsonString = "";                           // create a JSON string for sending data to the client
    JsonDocument doc;                      // create JSON container
    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    object["type"] = l_type;                          // write data into the JSON object -> I used "type" to identify if LED_selected or LED_intensity is sent and "value" for the actual value
    object["value"] = l_value;
    serializeJson(doc, jsonString);                // convert JSON object to string
    ws.textAll(jsonString);               // send JSON string to all clients
}
void sendJson(AsyncWebSocketClient * client, SystemPacket *sp) {
  JsonDocument doc;
  String s;
  doc["type"] = sp->cmd;
  doc["value"] = sp->param;
  serializeJson(doc,s);
  client->text(s);
}

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    DBGF2("ws[%s][%u] connect\n", server->url(), client->id());
    // client->printf("Hello Client %u :)", client->id());
    // client->ping();
    sendJson(client, "cmd_time", String(1709949899));
    sendJson(client, "cmd_mac", String(UNIT_MAC,HEX));
    sendJson(client, "cmd_ip", String(WiFi.localIP().toString()));
    sendJson(client, "cmd_dbg_level", String(DBG_LEVEL));
    sendJson(client, "printTime", String(TIME_PRINTTIME));
    sendJson(client, "espNowNbr1", String(ESPNOW_NEIGHBOR_1));
    sendJson(client, "espNowNbr2", String(ESPNOW_NEIGHBOR_2));
    sendJson(client, "loraNbr1", String(LORA_NEIGHBOR_1));
    sendJson(client, "loraNbr2", String(LORA_NEIGHBOR_2));

  } else if(type == WS_EVT_DISCONNECT){
    DBGF2("ws[%s][%u] disconnect\n", server->url(), client->id());
  } else if(type == WS_EVT_ERROR){
    DBGF2("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    DBGF2("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
      AwsFrameInfo * info = (AwsFrameInfo*)arg;
      String msg = "";
      if(info->final && info->index == 0 && info->len == len){
        //the whole message is in a single frame and we got all of it's data
        // DBGF2("ws[%s][%u] %s-message[%llu]: \n", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);

        if(info->opcode == WS_TEXT){
          for(size_t i=0; i < info->len; i++) {
            msg += (char) data[i];
          }
        } else {
          char buff[3];
          for(size_t i=0; i < info->len; i++) {
            sprintf(buff, "%02x ", (uint8_t) data[i]);
            msg += buff ;
          }
        }
        DBGF2("%s\n",msg.c_str());

        if(info->opcode == WS_TEXT) {
          // try to decipher the JSON string received
          JsonDocument doc;                    // create JSON container 
          DeserializationError error = deserializeJson(doc, msg.c_str());
          if (error) {
            DBG("deserializeJson() failed: ");
            DBG(error.f_str());
            return;
          }
          else {
            // JSON string was received correctly, so information can be retrieved:
            const char* l_type = doc["type"];
            DBG2("Type: " + String(l_type));
            if(String(l_type) == "cmd_time") {
              const unsigned long l_value = doc["value"];
              DBG2("Value: " + String(l_value));
              //setTime(l_value);
              // sendJson("cmd_time", String(now));
              sendJson(client, "cmd_time", String(1709949899));
            }
            if(String(l_type) == "cmd_dbg_level") {
              dbgLevelRuntime = doc["value"];
              DBG("Debug Level now " + String(dbgLevelRuntime));
            }
          // client->text("I got your text message");
          }
        }
        else {
          // client->binary("I got your binary message");
        }
      }
    else {
    //message is comprised of multiple frames or the frame is split into multiple packets
    if(info->index == 0){
        if(info->num == 0)
          DBGF2("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        DBGF2("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());

      if((info->index + len) == info->len){
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}

void begin_webserver() {
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  // events.onConnect([](AsyncEventSourceClient *client){
  //     client->send("hello!",NULL,millis(),1000);
  // });
  // server.addHandler(&events);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not found");
  });

  server.begin();
}

void handleWebserver() {
  ws.cleanupClients();

  static unsigned long updatePage = 0;
  if(TDIFF(updatePage,5000)) {
    // sendJson(String("cmd_dbg"),String("test"));
    // DBG("Test");
    //DBG2("Debug Level 2");
    //DBG1("Debug Level 1");
    // DBG("Debug Level 0");

    updatePage = millis();
  }
}