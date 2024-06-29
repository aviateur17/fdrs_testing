//  FARM DATA RELAY SYSTEM
//
//  GATEWAY 2.000
//
//  Developed by Timm Bogner (timmbogner@gmail.com) in Urbana, Illinois, USA.
//

/*
    Version History:
    1.1.0.0 main 20240628 - Merge FDRS changes into this project d6e63eb50bb96874b7b270cf9e45f29ff67fb22b
    1.0.0.0 main 20240627
*/

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

#ifdef HOST7488F0 // controller, ESP-NOW, Wrover ESP32, Address 85
    #include "host/host_7488f0.h"
#endif
#ifdef HOSTA94A3B // sensor, ESP-NOW, ESP01, Address 88
    #include "host/host_a94a3b.h"
#endif
#ifdef HOST368170 // sensor, LoRa, ESP32 w/ hallard shield, Address 0x90
    #include "host/host_368170.h"
#endif
#ifdef HOSTTTGO_LORA32_161 // sensor, LoRa, TTGO LORA32 V1.6.1, Address 0x91
    #include "host/host_ttgo_v161.h"
#endif
#ifdef HOSTTTGO_LORA32_13 // sensor, LoRa, TTGO LORA32 V1.3, Address 0x92
    #include "host/host_ttgo_v13.h"
#endif
#ifdef HOST8EBAB8 // sensor, ESP-NOW, ESP32s3 w/ OLED, Address 0x87
    #include "host/host_8ebab8.h"
#endif

#ifdef __FDRS_GATEWAYCONFIG_h__
    #include "fdrs_gateway_config.h"
#endif
#ifdef __FDRS_NODECONFIG_h__
    #include "fdrs_node_config.h"
#endif

const char* versionNumber = "1.1.0.0 main 20240628";

void setup() {
    delay(10000); // ESP32S3 takes several seconds to connect to COM port so do not see the initialization
    beginFDRS();
    DBGF("JL Version number: %s\n", versionNumber);
    host_setup(); // see hosts files and config files for #includes
}

void loop() {
    loopFDRS();
    host_loop(); // see hosts files and config files for #includes
}