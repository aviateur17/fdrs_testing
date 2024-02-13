#include <ArduinoJson.h>

#if defined (ESP32)
#define UART_IF Serial1
#define GPS_IF Serial2
#else
#define UART_IF Serial
#endif

extern time_t now;

void gpsParse(String input) {
  String _time, _date;
  int pos;
  struct tm gpsDateTime;
  static unsigned long lastGpsTimeSet = 0;

  // Prints out all incoming GPS data
  // DBG2("GPS incoming: " + input);

  // $GNZDA $GNZDA,154230.000,11,02,2024,00,00*4F
  // $GNRMC $GNRMC,154230.000,A,,,,,,,110224,,,A,V*19

  if(input.startsWith("$GNZDA") && input.length() >= 38) {
    // DBG2("GPS String: " + input + " Length: " + String(input.length()));
    pos = input.indexOf(",");
    _time = input.substring(pos + 1,pos + 7);
    // DBG2("GPS Time: " + _time + " ($GNZDA)");
    pos = input.indexOf(",",pos + 5);
    _date = input.substring(pos,pos + 11);
    _date.replace(",","");
    // DBG2("GPS Date: " + _date + " ($GNZDA)");

    // Set GPS time every 10 minutes
    if(lastGpsTimeSet == 0 || millis() - lastGpsTimeSet > (1000*60*10)) {    
      lastGpsTimeSet = millis();
      pos = 0;
      gpsDateTime.tm_hour = _time.substring(pos,pos+2).toInt();
      pos+=2;
      gpsDateTime.tm_min = _time.substring(pos,pos+2).toInt();
      pos+=2;
      gpsDateTime.tm_sec = _time.substring(pos,pos+2).toInt();
      pos = 0;
      gpsDateTime.tm_mday = _date.substring(pos,pos+2).toInt();
      pos+=2;
      gpsDateTime.tm_mon = _date.substring(pos,pos+2).toInt() - 1;
      pos+=2;
      gpsDateTime.tm_year = _date.substring(pos,pos+4).toInt() - 1900;
      DBG2("GPS Date & Time: " + String(gpsDateTime.tm_mon + 1) + "/" + String(gpsDateTime.tm_mday) + "/" + String(gpsDateTime.tm_year + 1900) + " " \
        + String(gpsDateTime.tm_hour) + ":" + String(gpsDateTime.tm_min) + ":" + String(gpsDateTime.tm_sec) + " UTC");
      DBG1("Setting date and time via GPS: " + String(mktime(&gpsDateTime)) + " $GNZDA");
      setTime(mktime(&gpsDateTime));
    }
  }
  else if(input.startsWith("$GNRMC") && input.length() >= 38) {

    // DBG2("GPS String: " + input + " Length: " + String(input.length()));
    pos = input.indexOf(",");
    _time = input.substring(pos + 1,pos + 7);
    // DBG2("GPS Time: " + _time + " ($GNRMC)");
    for(int i = 0 ; i < 8; i++) {
      pos = input.indexOf(",",pos + 1); 
    }
    _date = input.substring(pos,pos + 9);
    _date.replace(",","");
    // DBG2("GPS Date: " + _date + " ($GNRMC)");
    if(lastGpsTimeSet == 0 || millis() - lastGpsTimeSet > (1000*60*10)) {    
      lastGpsTimeSet = millis();
      pos = 0;
      gpsDateTime.tm_hour = _time.substring(pos,pos+2).toInt();
      pos+=2;
      gpsDateTime.tm_min = _time.substring(pos,pos+2).toInt();
      pos+=2;
      gpsDateTime.tm_sec = _time.substring(pos,pos+2).toInt();
      pos = 0;
      gpsDateTime.tm_mday = _date.substring(pos,pos+2).toInt();
      pos+=2;
      gpsDateTime.tm_mon = _date.substring(pos,pos+2).toInt() - 1;
      pos+=2;
      gpsDateTime.tm_year = 2000 + _date.substring(pos,pos+2).toInt() - 1900;
      DBG2("GPS Date & Time: " + String(gpsDateTime.tm_mon + 1) + "/" + String(gpsDateTime.tm_mday) + "/" + String(gpsDateTime.tm_year + 1900) + " " \
        + String(gpsDateTime.tm_hour) + ":" + String(gpsDateTime.tm_min) + ":" + String(gpsDateTime.tm_sec) + " UTC");
      DBG1("Setting date and time via GPS: " + String(mktime(&gpsDateTime)) + " $GNRMC");
      setTime(mktime(&gpsDateTime));
    }   
  }
  return;
}

void getSerial() {
  String incomingString;

  if (UART_IF.available()){
   incomingString =  UART_IF.readStringUntil('\n');
  }
  else if (Serial.available()){
   incomingString =  Serial.readStringUntil('\n');
  }
  if (GPS_IF.available()){
   incomingString =  GPS_IF.readStringUntil('\n');
   // Data is coming in every second from the GPS, let's minimize the processing power
   // required by only parsing periodically - maybe every 30 seconds.
   static unsigned long lastGpsParse = 0;
   if(lastGpsParse == 0 || millis() - lastGpsParse > (1000*30)) {
      lastGpsParse = millis();
      gpsParse(incomingString);
   }
   return;
  }
  DynamicJsonDocument doc(24576);
  DeserializationError error = deserializeJson(doc, incomingString);
  if (error) {    // Test if parsing succeeds.
    DBG2("json parse err");
    DBG2(incomingString);
    return;
  } else {
    int s = doc.size();
    JsonObject obj = doc[0].as<JsonObject>();
    if(obj.containsKey("type")) { // DataReading
    //UART_IF.println(s);
    for (int i = 0; i < s; i++) {
      theData[i].id = doc[i]["id"];
      theData[i].t = doc[i]["type"];
      theData[i].d = doc[i]["data"];
      }
      ln = s;
      newData = event_serial;
      DBG("Incoming Serial: DR");
      String data;
      serializeJson(doc, data);
      DBG1("Serial data: " + data);
    }
    else if(obj.containsKey("cmd")) { // SystemPacket
      cmd_t c = doc[0]["cmd"];
      if(c == cmd_time) {
        // Serial time master takes precedence over all others
        if(timeMaster.tmType == TM_NONE) {
          timeMaster.tmType = TM_SERIAL;
          timeMaster.tmAddress = 0x0000;
          DBG1("Time master is now Serial peer");
        }
        setTime(doc[0]["param"]); 
        DBG("Incoming Serial: time");
        timeMaster.tmLastTimeSet = millis();
      }
      else {
        DBG1("Incoming Serial: unknown cmd: " + String(c));
      }
    }
    else {    // Who Knows???
      DBG1("Incoming Serial: unknown: " + incomingString);
    }
  }
}


void sendSerial() {
  
  DynamicJsonDocument doc(24576);
  for (int i = 0; i < ln; i++) {
    doc[i]["id"]   = theData[i].id;
    doc[i]["type"] = theData[i].t;
    doc[i]["data"] = theData[i].d;
  }
  DBG("Sending Serial.");
  serializeJson(doc, UART_IF);
  UART_IF.println();
  String data;
  serializeJson(doc, data);
  DBG2("Serial data: " + data);

#ifndef ESP8266
  
#endif

}
void handleSerial(){
  while (UART_IF.available() || Serial.available() || GPS_IF.available())
  {
    getSerial();
  }
}

void sendTimeSerial() {
  
  DynamicJsonDocument SysPacket(64);
  SysPacket[0]["cmd"]   = cmd_time;
  SysPacket[0]["param"] = now;
  serializeJson(SysPacket, UART_IF);
  UART_IF.println();
  DBG("Sending Time via Serial.");
  String data;
  DBG2("Serial data: " + serializeJson(SysPacket, data));

#ifndef ESP8266
  // serializeJson(SysPacket, Serial);
  // Serial.println();
#endif
}

void begin_gps() {
  GPS_IF.begin(9600, SERIAL_8N1, GPS_RXD, GPS_TXD);
}