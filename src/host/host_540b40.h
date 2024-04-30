//  FARM DATA RELAY SYSTEM
//
//  GATEWAY CONFIGURATION

#define __FDRS_GATEWAYCONFIG_h__

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
#define USE_WEBSERVER
//#define USE_ETHERNET

// Actions
// Options: sendESPNowNbr(1 or 2); sendESPNowPeers(); sendLoRaNbr(1 or 2); broadcastLoRa(); sendSerial(); sendMQTT();
#define ESPNOWG_ACT    
#define LORAG_ACT      
#define SERIAL_ACT     sendMQTT();
#define MQTT_ACT        sendSerial();      
#define INTERNAL_ACT   sendMQTT();
#define ESPNOW1_ACT   
#define ESPNOW2_ACT 
#define LORA1_ACT      
#define LORA2_ACT 

// LoRa Configuration
#define RADIOLIB_MODULE SX1276
#define LORA_SS    RADIOLIB_NC
#define LORA_RST   RADIOLIB_NC
#define LORA_DIO   RADIOLIB_NC
#define LORA_BUSY  RADIOLIB_NC
#define LORA_TXPWR 2   // LoRa TX power in dBm (: +2dBm - +17dBm (for SX1276-7) +20dBm (for SX1278))
// //#define USE_SX126X

#define CUSTOM_SPI
#define LORA_SPI_SCK  RADIOLIB_NC
#define LORA_SPI_MISO RADIOLIB_NC
#define LORA_SPI_MOSI RADIOLIB_NC

#define FDRS_DEBUG     // Enable USB-Serial debugging
#define DBG_LEVEL 2
// #define DEBUG_CONFIG

// I2C - OLED or RTC
#define USE_I2C
#define I2C_SDA 21
#define I2C_SCL 22

// GPS UART Interface
// #define USE_GPS
#define GPS_RXD 25
#define GPS_TXD 27


// OLED -- Displays console debugging messages on an SSD1306 I²C OLED
// #define USE_OLED    
#define OLED_HEADER "FDRSGW"
#define OLED_PAGE_SECS 30
// #define OLED_SDA 21
// #define OLED_SCL 22
#define OLED_RST -1

// RTC - I2C
#define USE_RTC_DS3231
#define RTC_ADDR 0x57
// #define USE_RTC_DS1307
// #define RTC_ADDR 0x68


// UART data interface pins (if available)
#define RXD2 15
#define TXD2 14

//#define USE_LR  // Use ESP-NOW LR mode (ESP32 only)

// WiFi and MQTT Credentials  -- These will override the global settings
#define WIFI_SSID   AP_SSID
#define WIFI_PASS   AP_PASS

// Use Static IP Address for WiFi connections
#define USE_STATIC_IPADDRESS     
#define HOST_IPADDRESS      HOSTIPADDRESS
#define GW_IPADDRESS        GATEWAY
#define SUBNET_ADDRESS      SUBNET
#define DNS1_IPADDRESS      DNS1    // Required for NTP even if not using static IP defined below
#define DNS2_IPADDRESS      DNS2    // Optional. Only DNS1 is required to be able to use NTP


#define MQTT_ADDR   MQTT_SERVER
// #define MQTT_PORT   1883 // Default MQTT port is 1883
#define MQTT_AUTH   //Enable MQTT authentication 
// #define MQTT_USER   "user"
// #define MQTT_PASS   "pass"

// NTP Time settings
#define USDST
// #define EUDST
#define TIME_SERVER       "0.us.pool.ntp.org"       // NTP time server to use. If FQDN at least one DNS server is required to resolve name
#define STD_OFFSET      (-6)                // Local standard time offset in hours from UTC - if unsure, check https://time.is
#define DST_OFFSET      (STD_OFFSET + 1)    // Local savings time offset in hours from UTC - if unsure, check https://time.is
#define TIME_FETCHNTP     15      // Time, in minutes, between fetching time from NTP server, 0 = only on startup
#define TIME_PRINTTIME    1      // Time, in minutes, between printing local time to debug
#define TIME_SEND_INTERVAL 1    // Time, in minutes, between sending out time to remote devices

// Logging settings  --  Logging will occur when MQTT is disconnected
//#define USE_SD_LOG        //Enable SD-card logging
//#define USE_FS_LOG        //Enable filesystem (flash) logging
#define LOGBUF_DELAY 10000  // Log Buffer Delay - in milliseconds
#define SD_SS        0      //SD card CS pin (Use different pins for LoRa and SD)
#define LOG_FILENAME "fdrs_log.csv"

#include <fdrs_gateway.h>


void host_setup() {

}

void host_loop() {

}