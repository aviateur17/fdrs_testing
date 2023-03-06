//  FARM DATA RELAY SYSTEM
//
//  GATEWAY 2.000
//
//  Developed by Timm Bogner (timmbogner@gmail.com) in Urbana, Illinois, USA.
//

#include <Arduino.h>
#include "fdrs_gateway_config.h"
#include <fdrs_gateway.h>                                                                                                                                                                                                               

time_t lastRunTime = 0;
uint8_t LoRaAddress[] = { 0xFF, 0xFF };

void setup() {
beginFDRS();
}

void loop() {
    loopFDRS();
    // Send time to LoRa broadcast address every 5 minutes.
    // LoRa controllers should receive time and report via serial
    if(millis() - lastRunTime > (1000 * 60 * 5)) {
        #ifdef USE_LORA
        DBG("Sending LoRa time.");
        timeFDRSLoRa(LoRaAddress);
        #endif
        lastRunTime = millis();
    }
}