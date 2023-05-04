/*
   MIT License

  Copyright (c) 2021 Felix Biego

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

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
void get_sleep_time() {
  rtc.setTime(SECOND, MINUTES, HOURS, DAY, MONTH, YEAR);  // 17th Jan 2021 15:24:30
  if (HOURS <= wakeup_hour && MINUTES < wakeup_minute) {

    TIME_TO_SLEEP_SEC = ((wakeup_hour - HOURS) * 3600) + ((wakeup_minute - MINUTES) * 60);
  } else if (HOURS > wakeup_hour && MINUTES < wakeup_minute) {

    TIME_TO_SLEEP_SEC = ((HOURS + (HOURS - wakeup_hour)) * 3600) + ((wakeup_minute - MINUTES) * 60);
  }

}

void setup() {
  Serial.begin(115200);
  
}
  void loop() {
    get_sleep_time();
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  // (String) returns time with specified format
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_SEC * uS_TO_S_FACTOR);
    Serial.println("Going to sleep now");
    Serial.flush();
    esp_deep_sleep_start();
    wakeup_reason();
    

  }
