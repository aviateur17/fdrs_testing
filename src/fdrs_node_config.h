//  FARM DATA RELAY SYSTEM
//
//  Sensor Configuration

#ifndef __FDRS_NODECONFIG_h__
#define __FDRS_NODECONFIG_h__

#define READING_ID    31   //Unique ID for this sensor
#define GTWY_MAC      0x22 //Address of the nearest gateway

// #define USE_ESPNOW
#define USE_LORA
//#define DEEP_SLEEP
//#define POWER_CTRL    14

// Choose none or one of the debug options below.  
// DEBUG will show some information but for troubleshooting a good level would be FDRS_DEBUG_1
// #define FDRS_DEBUG     // Enable USB-Serial debugging
// #define FDRS_DEBUG_1 // Additional logging messages for troubleshooting purposes - commment out the other FDRS_DEBUG options
#define FDRS_DEBUG_2 // Show all logging messages for development purposes - comment out the other FDRS_DEBUG options

// I2C - OLED or RTC
#define I2C_SDA 21
#define I2C_SCL 22

// OLED -- Displays console debugging messages on an SSD1306 I²C OLED
#define USE_OLED    
#define OLED_HEADER "FDRS"
#define OLED_PAGE_SECS 30
#define OLED_RST -1

// LoRa Configuration
#define RADIOLIB_MODULE SX1276
#define LORA_SS    26
#define LORA_RST   RADIOLIB_NC
#define LORA_DIO   5
#define LORA_BUSY  RADIOLIB_NC
//#define USE_SX126X

#define LORA_TXPWR 2   // LoRa TX power in dBm (: +2dBm - +17dBm (for SX1276-7) +20dBm (for SX1278))
#define LORA_ACK        // Request LoRa acknowledgment.

// Time settings
#define USDST
// #define EUDST
#define STD_OFFSET      (-6)                // Local standard time offset in hours from UTC - if unsure, check https://time.is
#define DST_OFFSET      (STD_OFFSET + 1)    // Local savings time offset in hours from UTC - if unsure, check https://time.is
#define TIME_PRINTTIME    1     // Time, in minutes, between printing local time to debug

#endif // __FDRS_NODECONFIG_h__