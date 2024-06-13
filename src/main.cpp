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

#ifdef HOST8D7F60 // Gateway
    #include "host/host_8d7f60.h"
#endif
#ifdef HOST39EA44 // Gateway
    #include "host/host_39ea44.h"
#endif
#ifdef HOST393C24 // Gateway
    #include "host/host_393c24.h"
#endif
#ifdef HOST3607C8 // Gateway
    #include "host/host_3607c8.h"
#endif
#ifdef HOST540B40 // Gateway
    #include "host/host_540b40.h"
#endif
#ifdef HOST8D6214 // Gateway
    #include "host/host_8d6214.h"
#endif
#ifdef HOST8EB5D0 // Gateway
    #include "host/host_8eb5d0.h"
#endif

#ifdef HOST7488F0 // controller, ESP-NOW, Wrover ESP32
    #include "host/host_7488f0.h"
#endif
#ifdef HOSTA94A3B // sensor, ESP-NOW, ESP01
    #include "host/host_a94a3b.h"
#endif
#ifdef HOST368170 // sensor, LoRa, ESP32 w/ hallard shield
    #include "host/host_368170.h"
#endif
#ifdef HOSTTTGO_LORA32_161 // sensor, LoRa, TTGO LORA32 V1.6.1
    #include "host/host_ttgo_v161.h"
#endif
#ifdef HOSTTTGO_LORA32_13 // sensor, LoRa, TTGO LORA32 V1.3
    #include "host/host_ttgo_v13.h"
#endif
#ifdef HOST8EBAB8 // sensor, ESP-NOW, ESP32s3 w/ OLED
    #include "host/host_8ebab8.h"
#endif

#ifdef __FDRS_GATEWAYCONFIG_h__
    #include "fdrs_gateway_config.h"
#endif
#ifdef __FDRS_NODECONFIG_h__
    #include "fdrs_node_config.h"
#endif

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