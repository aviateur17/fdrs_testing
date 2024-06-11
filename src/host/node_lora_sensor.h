//  FARM DATA RELAY SYSTEM
//
//  LoRa Sensor Example
//
//  Developed by Timm Bogner (timmbogner@gmail.com) in Urbana, Illinois, USA.
//  An example of how to send data via LoRa using FDRS.
//
#include <fdrs_node.h>

float data1;
float data2;


float readTemp() {
  return 21.0;
}

float readHum() {
  return random(0,100);
}


void host_setup() {
  // beginFDRS(); // Already called by setup() in main.cpp
}
void host_loop() {
  data1 = readHum();
  loadFDRS(data1, HUMIDITY_T);
  data2 = readTemp();
  loadFDRS(data2, TEMP_T);
  sendFDRS();
  sleepFDRS(10);  //Sleep time in seconds
}