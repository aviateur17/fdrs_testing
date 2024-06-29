
#include <fdrs_gateway.h>
#include <driver/gpio.h>

#define LED GPIO_NUM_21
#define LEDON 0 
#define LEDOFF 1

// TaskHandle_t tskLedFlash;
// int ledBlinks = 2;
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

void doLedFlash(void * param) {
  int numFlashes = 0;

  ESP_LOGI(TAG,"LED TASK STARTING");

  gpio_set_level(LED, LEDOFF);
  delay(88);
  // vTaskDelay(88/portTICK_PERIOD_MS);
  gpio_set_level(LED, LEDON);
  delay(88);
  // vTaskDelay(88/portTICK_PERIOD_MS);
  gpio_set_level(LED, LEDOFF);

  if(param == NULL)
    numFlashes = 0;
  else
    numFlashes = *((int*) param);

  while(true) {

    for(int i = 1; i <= numFlashes; i++) {
      gpio_set_level(LED, LEDOFF);
      // vTaskDelay(88/portTICK_PERIOD_MS);
      delay(88);
      gpio_set_level(LED, LEDON);
      //vTaskDelay(88/portTICK_PERIOD_MS);
      delay(88);
      gpio_set_level(LED, LEDOFF);
    }
    //vTaskDelay(2000/portTICK_PERIOD_MS);
    delay(2000);
  }
  ESP_LOGE(TAG,"Deleting LED TASK");
  vTaskDelete(NULL);
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
    // xTaskCreatePinnedToCore(&doLedFlash, "LedFlash", 2048, (void *) &ledBlinks, 0, &tskLedFlash, app_cpu);

}

void host_loop() {
  ledblink(2);
}