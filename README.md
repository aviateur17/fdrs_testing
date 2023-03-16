Based off of: https://github.com/timmbogner/Farm-Data-Relay-System


Jeff Testing Branch for FDRS Gateway with PlatformIO in VSCode

Host 540b40
WiFi -> MQTT & UART <-->

#define UNIT_MAC           0xde


#define SERIAL_ACT     sendMQTT();
#define MQTT_ACT       sendSerial();   
#define INTERNAL_ACT   sendMQTT();