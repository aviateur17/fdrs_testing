//  FARM DATA RELAY SYSTEM
//
//  GATEWAY 2.000
//
//  Developed by Timm Bogner (timmbogner@gmail.com) in Urbana, Illinois, USA.
//

#include <Arduino.h>
#include "hostdefs.h"
#include "credentials.h"
#include "fdrs_gateway_config.h"
#include <fdrs_gateway.h>   

#define MS2MIN 1000*60
#define MS2SEC 1000

unsigned long lastRunTime = 0;

void setup() {
    beginFDRS();
    lastRunTime = millis();
}

void loop() {
    loopFDRS();
    // if(millis() - lastRunTime > (1000*60*1)) {
    //     loadFDRS(random(0,100),0,82);
    //     loadFDRS(random(0,100),0,82);
    //     loadFDRS(random(0,100),0,82);
    //     lastRunTime = millis();
    //     sendFDRS();
    // }

    // Testing time sending - why are recipiient reciving time twice on ESPNOW???
    if(millis() - lastRunTime > (MS2SEC*20)) {
        lastRunTime = millis();
        sendTime();
    }
}