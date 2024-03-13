
#include <fdrs_gateway.h>

#define LED GPIO_NUM_2
#define LEDON 1
#define LEDOFF 0

unsigned long ledtime;

// initialize LED on GPIO2 as output and Pull down
void initLed(gpio_num_t pin) {
  gpio_config_t gpioLed;
  gpioLed.pin_bit_mask = ((1ULL<< pin));
  gpioLed.mode         = GPIO_MODE_OUTPUT;
  gpioLed.pull_up_en   = GPIO_PULLUP_DISABLE;
  gpioLed.pull_down_en = GPIO_PULLDOWN_ENABLE;
  gpioLed.intr_type    = GPIO_INTR_DISABLE;
  ESP_ERROR_CHECK(gpio_config(&gpioLed));
}

void ledblink(uint numBlinks) {
  if(millis() - ledtime > 2000) {
    for(int i=0; i<numBlinks; i++) {
      ledtime = millis();
      gpio_set_level(LED, LEDOFF);
      while(millis() - ledtime < 88) { }
      ledtime = millis();
      gpio_set_level(LED, LEDON);
      while(millis() - ledtime < 88) { }
      ledtime = millis();
      gpio_set_level(LED, LEDOFF);
    }
  }
}

void host_setup() {
    initLed(LED);
}

void host_loop() {
  ledblink(2);
}