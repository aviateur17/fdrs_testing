//  FARM DATA RELAY SYSTEM
//
//  GATEWAY 2.000
//
//  Developed by Timm Bogner (timmbogner@gmail.com) in Urbana, Illinois, USA.
//

#include <Arduino.h>
#include "credentials.h"
#include "hostdefs.h"
#include "fdrs_gateway_config.h"
#include <fdrs_gateway.h>   

void someData() {
    loadFDRS(55.243,2,0x23);
    sendFDRS();
}

void setup() {
    beginFDRS();
    // send some data every minute
    scheduleFDRS(someData,1000*60);
}

void loop() {
    static unsigned long t = 0;
    loopFDRS();
    // // Send time to LoRa broadcast address every 5 minutes.
    // // LoRa controllers should receive time and report via serial
    // if(millis() - lastRunTime > (1000 * 60 * 5)) {
    //     #ifdef USE_LORA
    //     DBG("Sending LoRa time.");
    //     timeFDRSLoRa(LoRaAddress);
    //     #endif
    //     lastRunTime = millis();
    // }
}