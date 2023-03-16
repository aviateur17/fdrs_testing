#include <ArduinoJson.h>

#if defined (ESP32)
#define UART_IF Serial1
#else
#define UART_IF Serial
#endif

void getSerial() {
  String incomingString;
  int timeIndex = 0;
  bool serialTimeFlag = false;

  if (UART_IF.available()){
   incomingString =  UART_IF.readStringUntil('\n');
  }
  else if (Serial.available()){
   incomingString =  Serial.readStringUntil('\n');
  }
  // DBG(incomingString);
  DynamicJsonDocument doc(24576);
  DeserializationError error = deserializeJson(doc, incomingString);
  if (error) {    // Test if parsing succeeds.
    //    DBG("json parse err");
    //    DBG(incomingString);
    return;
  } else {
    int s = doc.size();
    //UART_IF.println(s);
    for (int i = 0; i < s; i++) {
      theData[i].id = doc[i]["id"];
      theData[i].t = doc[i]["type"];
      theData[i].d = doc[i]["data"];
      if(theData[i].t == TIME_T) {
        timeIndex = i;
        serialTimeFlag = true;
      }
    }
    ln = s;
    if(serialTimeFlag) {
      DBG("Incoming time via Serial.");
      time_t previousTime = now;
      now = theData[timeIndex].d;
      setTime(previousTime); 
      timeIndex = 0;
      serialTimeFlag = false;
    }
    newData = event_serial;
    DBG("Incoming Serial.");
  }
}

void sendSerial() {
  DBG("Sending Serial.");
  DynamicJsonDocument doc(24576);
  for (int i = 0; i < ln; i++) {
    doc[i]["id"]   = theData[i].id;
    doc[i]["type"] = theData[i].t;
    doc[i]["data"] = theData[i].d;
  }
  serializeJson(doc, UART_IF);
  UART_IF.println();

#ifndef ESP8266
  serializeJson(doc, Serial);
  Serial.println();
#endif

}
void handleSerial(){
  while (UART_IF.available() || Serial.available())
  {
    getSerial();
  }
}

void sendTimeSerial() {
  theData[0].id = 0;
  theData[0].t = TIME_T;
  theData[0].d = now;
  ln = 1;
  sendSerial();
}