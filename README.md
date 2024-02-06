Based off of: https://github.com/timmbogner/Farm-Data-Relay-System


Jeff Testing Branch for FDRS Gateway with PlatformIO in VSCode

Host 8d6214
WiFi <--> Serial Gateway

//Addresses
#define UNIT_MAC           0x20  // The address of this gateway

#define ESPNOW_NEIGHBOR_1  0x00  // Address of ESP-NOW neighbor #1
#define ESPNOW_NEIGHBOR_2  0x00  // Address of ESP-NOW neighbor #2
#define LORA_NEIGHBOR_1    0x00  // Address of LoRa neighbor #1
#define LORA_NEIGHBOR_2    0x00  // Address of LoRa neighbor #2

// Interfaces
// #define USE_ESPNOW  
// #define USE_LORA
#define USE_WIFI  // Will cause errors if used with ESP-NOW. Use a serial link instead!
//#define USE_ETHERNET

// Actions
// Options: sendESPNowNbr(1 or 2); sendESPNowPeers(); sendLoRaNbr(1 or 2); broadcastLoRa(); sendSerial(); sendMQTT();
#define ESPNOWG_ACT    
#define LORAG_ACT      
#define SERIAL_ACT     sendMQTT();
#define MQTT_ACT       sendSerial();
#define INTERNAL_ACT   sendMQTT();
#define ESPNOW1_ACT    
#define ESPNOW2_ACT                    
#define LORA1_ACT      
#define LORA2_ACT 