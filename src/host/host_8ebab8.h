//  FARM DATA RELAY SYSTEM
//
//  Sensor Configuration

#define __FDRS_NODECONFIG_h__

#define READING_ID    87   //Unique ID for this sensor
#define GTWY_MAC      0x21 //Address of the nearest gateway

#define USE_ESPNOW
// #define USE_LORA
// #define DEEP_SLEEP
//#define POWER_CTRL    14

#define FDRS_DEBUG
#define DBG_LEVEL 2
#define DEBUG_CONFIG // Displays full config info on startup

// OLED -- Displays console debugging messages on an SSD1306 I²C OLED
#define USE_OLED    
#define OLED_HEADER "FDRS87"
#define OLED_PAGE_SECS 30
#define OLED_RST GPIO_NUM_NC

// I2C - OLED or RTC
#define USE_I2C
#define I2C_SDA 4
#define I2C_SCL 5

// LoRa Configuration
#define RADIOLIB_MODULE SX1276 //Tested on SX1276
#define LORA_SS GPIO_NUM_NC
#define LORA_RST GPIO_NUM_NC
#define LORA_DIO GPIO_NUM_NC

#define LORA_TXPWR 2    // LoRa TX power in dBm (: +2dBm - +17dBm (for SX1276-7) +20dBm (for SX1278))
#define LORA_ACK        // Request LoRa acknowledgment.

#define CUSTOM_SPI
#define LORA_SPI_SCK GPIO_NUM_NC
#define LORA_SPI_MISO GPIO_NUM_NC
#define LORA_SPI_MOSI GPIO_NUM_NC

// NTP Time settings
#define USDST
// #define EUDST
#define TIME_SERVER       "0.us.pool.ntp.org"       // NTP time server to use. If FQDN at least one DNS server is required to resolve name
#define STD_OFFSET      (-6)                // Local standard time offset in hours from UTC - if unsure, check https://time.is
#define DST_OFFSET      (STD_OFFSET + 1)    // Local savings time offset in hours from UTC - if unsure, check https://time.is
#define TIME_FETCHNTP     15      // Time, in minutes, between fetching time from NTP server
#define TIME_PRINTTIME    1      // Time, in minutes, between printing local time to debug
#define TIME_SEND_INTERVAL 5    // Time, in minutes, between sending out time to remote devices


/////////////////////////////////////////
#include <fdrs_node.h>

#define PIRPIN GPIO_NUM_9
#define LED GPIO_NUM_21
#define LEDON 0
#define LEDOFF 1
#define SERIAL_PORT Serial
#define MSTOMINS (1000 * 60)
#define MSTOSECS (1000)

time_t lastruntime, pirOnTime, pirOffTime, lastPingTime;
bool pirCurrentState = 0;
uint pirOnDelaySecs = 10;
uint pirOffDelaySecs = 10;

// void initPir(gpio_num_t pin) {
//   gpio_config_t gpioPir;
//   gpioPir.pin_bit_mask = ((1ULL<< pin));
//   gpioPir.mode         = GPIO_MODE_INPUT;
//   gpioPir.pull_up_en   = GPIO_PULLUP_DISABLE;
//   gpioPir.pull_down_en = GPIO_PULLDOWN_DISABLE;
//   gpioPir.intr_type    = GPIO_INTR_DISABLE;
//   ESP_ERROR_CHECK(gpio_config(&gpioPir));
// }

// void initLed(gpio_num_t pin) {
//   gpio_config_t gpioLed;
//   gpioLed.pin_bit_mask = ((1ULL<< pin));
//   gpioLed.mode         = GPIO_MODE_OUTPUT;
//   gpioLed.pull_up_en   = GPIO_PULLUP_DISABLE;
//   gpioLed.pull_down_en = GPIO_PULLDOWN_ENABLE;
//   gpioLed.intr_type    = GPIO_INTR_DISABLE;
//   ESP_ERROR_CHECK(gpio_config(&gpioLed));
// }

// void initPir() {
//   pinMode(9, INPUT);
// }
// void initLed() {
//   pinMode(21, INPUT);
// }

// bool loopPir() {
//   bool sendResult;

//   if(gpio_get_level(PIRPIN) && pirCurrentState == 0 && (millis() - pirOffTime > (pirOffDelaySecs * MSTOSECS))) {
//     gpio_set_level(LED, LEDON);
//     DBG("PIR High");
//     pirCurrentState = 1;
//     pirOnTime = millis();
//     loadFDRS(1, STATUS_T);
//     sendResult = sendFDRS();
//     gpio_set_level(LED, LEDON);
//   }
//   else if(!gpio_get_level(PIRPIN) && pirCurrentState == 1 && (millis() - pirOnTime > (pirOnDelaySecs * MSTOSECS))) {
//     gpio_set_level(LED, LEDOFF);
//     DBG("PIR Low");
//     pirCurrentState = 0;
//     pirOffTime = millis();
//     loadFDRS(0, STATUS_T);
//     sendResult = sendFDRS();
//     gpio_set_level(LED, LEDOFF);
//   }
  // if(digitalRead(9) && pirCurrentState == 0 && (millis() - pirOffTime > (pirOffDelaySecs * MSTOSECS))) {
  //   digitalWrite(21, LEDON);
  //   DBG("PIR High");
  //   pirCurrentState = 1;
  //   pirOnTime = millis();
  //   loadFDRS(1, STATUS_T);
  //   sendResult = sendFDRS();
  //   digitalWrite(21, LEDON);
  // }
  // else if(!digitalRead(9) && pirCurrentState == 1 && (millis() - pirOnTime > (pirOnDelaySecs * MSTOSECS))) {
  //   digitalWrite(21, LEDOFF);
  //   DBG("PIR Low");
  //   pirCurrentState = 0;
  //   pirOffTime = millis();
  //   loadFDRS(0, STATUS_T);
  //   sendResult = sendFDRS();
  //   digitalWrite(21, LEDOFF);
  // }
//   return sendResult;
// }

unsigned long lastRunTime = 0;

void fdrs_recv_cb(DataReading theData)
{
  DBG("ID: " + String(theData.id));
  DBG("Type: " + String(theData.t));
  DBG("Data: " + String(theData.d));
}

void host_setup() {

    // initPir(PIRPIN);
    // initLed(LED);

    addFDRS(fdrs_recv_cb);
    subscribeFDRS(READING_ID);
    reqTimeFDRS();
    pingFDRS(500);
    
}

void host_loop() {
    // loopPir();
    if(TDIFFRAND(lastRunTime,60000)) {
        loadFDRS(READING_ID, STATUS_T);
        if(sendFDRS()){
            DBG("Big Success!");
        } else {
            DBG("Nope, not so much.");
        }
        // DBG("PIR: " + String(gpio_get_level(PIRPIN)));
        // sleepFDRS(10);
        lastRunTime = millis();
    }
}