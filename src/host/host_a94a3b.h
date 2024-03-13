//  FARM DATA RELAY SYSTEM
//
//  Sensor Configuration

#define __FDRS_NODECONFIG_h__

#define READING_ID    88   //Unique ID for this sensor
#define GTWY_MAC      0x21 //Address of the nearest gateway

#define USE_ESPNOW
// #define USE_LORA
// #define DEEP_SLEEP
//#define POWER_CTRL    14
#define FDRS_DEBUG
#define DBG_LEVEL 2
// #define DEBUG_CONFIG // Displays full config info on startup

// OLED -- Displays console debugging messages on an SSD1306 I²C OLED
// #define USE_OLED    
#define OLED_HEADER "FDRSSN"
#define OLED_RST GPIO_NUM_NC
#define OLED_PAGE_SECS 30

// I2C - OLED or RTC
// #define USE_I2C
#define I2C_SDA 1
#define I2C_SCL 2


// LoRa Configuration
#define RADIOLIB_MODULE SX1276 //Tested on SX1276
#define LORA_SS 26
#define LORA_RST 27
#define LORA_DIO 5

#define LORA_TXPWR 2    // LoRa TX power in dBm (: +2dBm - +17dBm (for SX1276-7) +20dBm (for SX1278))
#define LORA_ACK        // Request LoRa acknowledgment.

#define CUSTOM_SPI
#define LORA_SPI_SCK 18
#define LORA_SPI_MISO 19
#define LORA_SPI_MOSI 23

// NTP Time settings
#define USDST
// #define EUDST
#define TIME_SERVER       "0.us.pool.ntp.org"       // NTP time server to use. If FQDN at least one DNS server is required to resolve name
#define STD_OFFSET      (-6)                // Local standard time offset in hours from UTC - if unsure, check https://time.is
#define DST_OFFSET      (STD_OFFSET + 1)    // Local savings time offset in hours from UTC - if unsure, check https://time.is
#define TIME_FETCHNTP     15      // Time, in minutes, between fetching time from NTP server
#define TIME_PRINTTIME    1      // Time, in minutes, between printing local time to debug
#define TIME_SEND_INTERVAL 5    // Time, in minutes, between sending out time to remote devices

#include <fdrs_node.h>
#include "host/dhtsensor.h"

void host_setup() {
    beginFDRS();
    dht_setup();
}

void host_loop() {

    loopFDRS();
    pingFDRS(500);
    reqTimeFDRS();
    dht_loop();
    sleepFDRS(120 + random(-10,10));  //Sleep time in seconds
}