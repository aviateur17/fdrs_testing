//  FARM DATA RELAY SYSTEM
//
//  GATEWAY 2.000
//
//  Developed by Timm Bogner (timmbogner@gmail.com) in Urbana, Illinois, USA.
//

#include <Arduino.h>

// Use only core 1
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t pro_cpu = 0;
  static const BaseType_t app_cpu = 1;
#endif   
static const char TAG[] = __FILE__;

#include "host/credentials.h"
#include "host/hostdefs.h"

#ifdef HOST8D7F60
    #include "host/host_8d7f60.h"
#endif
#ifdef HOST39EA44
    #include "host/host_39ea44.h"
#endif
#ifdef HOST393C24
    #include "host/host_393c24.h"
#endif
#ifdef HOST3607C8
    #include "host/host_3607c8.h"
#endif
#ifdef HOST540B40
    #include "host/host_540b40.h"
#endif
#ifdef HOST8D6214
    #include "host/host_8d6214.h"
#endif
#ifdef HOST8EB5D0
    #include "host/host_8eb5d0.h"
#endif
#include "fdrs_gateway_config.h"

time_t lastRunTime = 0;

void setup() {
    delay(10000); // ESP32S3 takes several seconds to connect to COM port so do not see the initialization
    beginFDRS();
    host_setup();
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

    host_loop();
}