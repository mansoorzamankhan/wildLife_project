#ifndef _SLEEP_H_
#define _SLEEP_H_

#include <ESP32Time.h>
#define wakeup_hour 11
#define wakeup_minute 31
#define HOURS 11
#define MINUTES 30
#define SECOND 30
#define YEAR 2023
#define MONTH 5
#define DAY 4
#define uS_TO_S_FACTOR 1000000ULL   /* Conversion factor for micro seconds to seconds */
unsigned int TIME_TO_SLEEP_SEC = 0; /* Time ESP32 will go to sleep (in seconds) */
ESP32Time rtc;


void wakeup_reason();
void set_RTC_and_sleep_time();
void go_deep_sleep();

void wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default:
      Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);



      //rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
      //rtc.offset = 7200; // change offset value

      break;
  }
}
void set_RTC_and_sleep_time() {
  rtc.setTime(SECOND, MINUTES, HOURS, DAY, MONTH, YEAR);  // 17th Jan 2021 15:24:30
  if (HOURS <= wakeup_hour && MINUTES < wakeup_minute) {

    TIME_TO_SLEEP_SEC = ((wakeup_hour - HOURS) * 3600) + ((wakeup_minute - MINUTES) * 60);
  } else if (HOURS > wakeup_hour && MINUTES < wakeup_minute) {

    TIME_TO_SLEEP_SEC = ((HOURS + (HOURS - wakeup_hour)) * 3600) + ((wakeup_minute - MINUTES) * 60);
  }
}
void go_deep_sleep() {
  Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  // (String) returns time with specified format
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_SEC * uS_TO_S_FACTOR);
  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
  wakeup_reason();
}
#endif
