Based off of: https://github.com/timmbogner/Farm-Data-Relay-System


Jeff Testing Branch for FDRS Gateway with PlatformIO in VSCode

Host 3607c8, MAC 0x21
UART <--> ESP-NOW

ESP-NOW Neighbor1 0x22 (ESP-NOW -> LoRa gateway)

#define UNIT_MAC           0x21

#define ESPNOWG_ACT    sendSerial();
#define LORAG_ACT      
#define SERIAL_ACT     sendESPNowPeers();
#define MQTT_ACT          
#define INTERNAL_ACT   sendSerial();
#define ESPNOW1_ACT    sendSerial();
#define ESPNOW2_ACT    sendESPNowNbr(1);                
#define LORA1_ACT      
#define LORA2_ACT 