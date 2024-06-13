Based off of: https://github.com/timmbogner/Farm-Data-Relay-System


Jeff Testing Branch for FDRS with PlatformIO in VSCode

Host 39ea44
ESP-NOW <--> LoRa Gateway

#define UNIT_MAC           0x22

ESP-NOW Neighbor 1 0x21

#define ESPNOWG_ACT    sendESPNowNbr(1);
#define LORAG_ACT      sendESPNowNbr(1);
#define SERIAL_ACT     
#define MQTT_ACT          
#define INTERNAL_ACT   sendESPNowNbr(1);
#define ESPNOW1_ACT    sendESPNowNbr(2); sendESPNowPeers(); broadcastLoRa();
#define ESPNOW2_ACT    sendESPNowNbr(1);                
#define LORA1_ACT      
#define LORA2_ACT 