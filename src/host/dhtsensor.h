//  FARM DATA RELAY SYSTEM
//
//  ESP-NOW Sensor Example
//
//  Developed by Timm Bogner (timmbogner@gmail.com) for Sola Gratia Farm in Urbana, Illinois, USA.
//  An example of how to send data via ESP-NOW using FDRS.
//

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor 

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

#define ERRVAL 999.99
#define TEMPOFFSET 0
#define HUMOFFSET 0
#define SERIAL_PORT Serial

void dht_setup() {
  dht.begin();
  randomSeed(analogRead(0));
  SERIAL_PORT.printf("MAC Address: %s\n", WiFi.macAddress().c_str());
}
void dht_loop() {

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    loadFDRS(ERRVAL, TEMP_T);
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    loadFDRS(event.temperature, TEMP_T);
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    loadFDRS(ERRVAL, HUMIDITY_T);
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    loadFDRS(event.relative_humidity, HUMIDITY_T);
  }

  if(sendFDRS()){
    DBG("Big Success!");
  } else {
    DBG("Nope, not so much.");
  }
}
