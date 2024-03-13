// #include <WebServer.h>
// #include <WebSocketsServer.h>

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

// https://htmlcompressor.com/compressor/
// The String below "webpage" contains the complete HTML code that is sent to the client whenever someone connects to the webserver
// NOTE 27.08.2022: I updated in the webpage "slider.addEventListener('click', slider_changed);" to "slider.addEventListener('change', slider_changed);" -> the "change" did not work on my phone.
// String webpage = "<!DOCTYPE html><html><head><title>FDRS</title></head><body style='background-color: #EEEEEE;'><span style='color: #003366;'><h1>LED Controller</h1><form> <p>Select LED:</p> <div> <input type='radio' id='ID_LED_0' name='operation_mode'> <label for='ID_LED_0'>LED 0</label> <input type='radio' id='ID_LED_1' name='operation_mode'> <label for='ID_LED_1'>LED 1</label> <input type='radio' id='ID_LED_2' name='operation_mode'> <label for='ID_LED_2'>LED 2</label> </div></form><br>Set intensity level: <br><input type='range' min='1' max='100' value='50' class='slider' id='LED_INTENSITY'>Value: <span id='LED_VALUE'>-</span><br></span></body><script> document.getElementById('ID_LED_0').addEventListener('click', led_changed); document.getElementById('ID_LED_1').addEventListener('click', led_changed); document.getElementById('ID_LED_2').addEventListener('click', led_changed); var slider = document.getElementById('LED_INTENSITY'); var output = document.getElementById('LED_VALUE'); slider.addEventListener('change', slider_changed); var Socket; function init() { Socket = new WebSocket('ws://' + window.location.hostname + ':81/'); Socket.onmessage = function(event) { processCommand(event); }; } function led_changed() {var l_LED_selected = 0;if(document.getElementById('ID_LED_1').checked == true) { l_LED_selected = 1;} else if(document.getElementById('ID_LED_2').checked == true) { l_LED_selected = 2;}console.log(l_LED_selected); var msg = { type: 'LED_selected', value: l_LED_selected};Socket.send(JSON.stringify(msg)); } function slider_changed () { var l_LED_intensity = slider.value;console.log(l_LED_intensity);var msg = { type: 'LED_intensity', value: l_LED_intensity};Socket.send(JSON.stringify(msg)); } function processCommand(event) {var obj = JSON.parse(event.data); var type = obj.type;if (type.localeCompare(\"LED_intensity\") == 0) { var l_LED_intensity = parseInt(obj.value); console.log(l_LED_intensity); slider.value = l_LED_intensity; output.innerHTML = l_LED_intensity;}else if(type.localeCompare(\"LED_selected\") == 0) { var l_LED_selected = parseInt(obj.value); console.log(l_LED_selected); if(l_LED_selected == 0) { document.getElementById('ID_LED_0').checked = true; } else if (l_LED_selected == 1) { document.getElementById('ID_LED_1').checked = true; } else if (l_LED_selected == 2) { document.getElementById('ID_LED_2').checked = true; }} } window.onload = function(event) { init(); }</script></html>";
// String webpage = "<!DOCTYPE html> <html> <head> <title>FDRS Gateway</title> </head> <body style=background-color:#eee> <span style=color:#036> <h1>FDRS Gateway</h1> <form> <div> <br> <fieldset> <legend>Time Set:</legend> <br>PC Time: <p id=pctime></p> <br> <br>Controller Time: <p id=ctrlrtime></p> <br> <input type=button id=butsettime name=butsettime value='Set Time' onclick=btnTimePressed()> </fieldset> <br><br> <fieldset> <legend>Debug Messages</legend> <textarea name=message style=width:600px;height:100px>Future DBG messages???</textarea> </fieldset> </div> </form> <script>var Socket;var espTime=new Date();function init(){Socket=new WebSocket('ws://'+window.location.hostname+':81/');Socket.onmessage=function(a){processCommand(a)}}function processCommand(b){var c=JSON.parse(b.data);var a=c.type;if(a.localeCompare(\"cmd_time\")==0){espTime=Date(parseInt(c.value));document.getElementById(\"ctrlrtime\").innerHTML=espTime}else{console.log(a)}}setInterval(pcDate,1000);function pcDate(){document.getElementById(\"pctime\").innerHTML=Date()}function btnTimePressed(){}window.onload=function(a){init()};</script> </body> </html>";
#include "fdrs_webpage.h"

// Type can be LED_intensity or LED_selected
// Value either which LED or the intensity value

// Initialization of webserver and websocket
// WebServer server(80);                                 // the server uses port 80 (standard port for websites
// WebSocketsServer webSocket = WebSocketsServer(81);    // the websocket uses port 81 (standard port for websockets

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");

// Simple function to send information to the web clients
void sendJson(String l_type, String l_value) {
    String jsonString = "";                           // create a JSON string for sending data to the client
    JsonDocument doc;                      // create JSON container
    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    object["type"] = l_type;                          // write data into the JSON object -> I used "type" to identify if LED_selected or LED_intensity is sent and "value" for the actual value
    object["value"] = l_value;
    serializeJson(doc, jsonString);                // convert JSON object to string
    webSocket.broadcastTXT(jsonString);               // send JSON string to all clients
}
void sendJson(SystemPacket *sp) {
  JsonDocument doc;
  String s;
  doc["type"] = sp->cmd;
  doc["value"] = sp->param;
  serializeJson(doc,s);
  webSocket.broadcastTXT(s);
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {      // the parameters of this callback function are always the same -> num: id of the client who send the event, type: type of message, payload: actual data sent and length: length of payload
  switch (type) {                                     // switch on the type of information sent
    case WStype_DISCONNECTED:                         // if a client is disconnected, then type == WStype_DISCONNECTED
      DBG2("Client " + String(num) + " disconnected");
      break;
    case WStype_CONNECTED:                            // if a client is connected, then type == WStype_CONNECTED
      DBG2("Client " + String(num) + " connected");
      // optionally you can add code here what to do when connected
      sendJson("cmd_time", String(now));
      sendJson("cmd_mac", String(UNIT_MAC,HEX));
      sendJson("cmd_ip",String(WiFi.localIP().toString()));
      break;
    case WStype_TEXT:                                 // if a client has sent data, then type == WStype_TEXT
      // try to decipher the JSON string received
      JsonDocument doc;                    // create JSON container 
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      else {
        // JSON string was received correctly, so information can be retrieved:
        const char* l_type = doc["type"];
        const unsigned long l_value = doc["value"];
        Serial.println("Type: " + String(l_type));
        Serial.println("Value: " + String(l_value));

        if(String(l_type) == "cmd_time") {
          DBG(String(l_value));
          //setTime(l_value);
          // sendJson("cmd_time", String(now));
          sendJson("cmd_time", String(1709949899));
        }
        // if LED_intensity value is received -> update and write to LED
        else if(String(l_type) == "LED_intensity") {
          LED_intensity = int(l_value);
          sendJson("LED_intensity", String(l_value));
          ledBlinks = map(LED_intensity,0,100,0,6);
          // ledcWrite(led_channels[LED_selected], map(LED_intensity, 0, 100, 0, 255));
        }
        // else if LED_select is changed -> switch on LED and switch off the rest
        else if(String(l_type) == "LED_selected") {
          LED_selected = int(l_value);
          sendJson("LED_selected", String(l_value));
          if(LED_selected == 0) {
            ledBlinks = map(LED_intensity,0,100,0,6);
          }
          else {
            ledBlinks = 0;
          }
        }
        else {
          DBG("Type: " + String(l_type) + " Value: " + String(l_value));
        }
      }

      break;
  }
}

void begin_webserver() {
  server.on("/", []() {                               // define here wat the webserver needs to do
    server.send(200, "text/html", webpage);           //    -> it needs to send out the HTML string "webpage" to the client
  });
  server.begin();                                     // start server
  
  webSocket.begin();                                  // start websocket
  webSocket.onEvent(webSocketEvent);   
}

void handleWebserver() {
  server.handleClient();                              // Needed for the webserver to handle all clients
  webSocket.loop();                                   // Update function for the webSockets 
  
  // static unsigned long updatePage = 0;
  // if(TDIFF(updatePage,5000)) {
  
  //   String jsonString = "";                           // create a JSON string for sending data to the client
  //   JsonDocument doc;                      // create a JSON container
  //   JsonObject object = doc.to<JsonObject>();         // create a JSON Object
  //   object["rand1"] = random(100);                    // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
  //   object["rand2"] = random(100);
  //   serializeJson(doc, jsonString);                   // convert JSON object to string
  //   Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
  //   webSocket.broadcastTXT(jsonString);               // send JSON string to clients

  //   updatePage = millis();
  // }
}