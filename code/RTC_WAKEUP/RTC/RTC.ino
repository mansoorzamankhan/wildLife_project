#include <WiFi.h>
#include <esp_sleep.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  rtc.begin();

  // Set the alarm to 7:00 AM every day
  DateTime now = rtc.now();
  DateTime alarmTime(now.year(), now.month(), now.day()+1, 7, 0, 0);
  rtc.setAlarm1(alarmTime, DS3231_A1_Hour);

  // Enable the alarm interrupt
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 0);
  esp_sleep_enable_ext1_wakeup(BIT64(ESP_SLEEP_WAKEUP_EXT1));


  esp_deep_sleep_start();
}

void loop() {
  // This code will not be executed as the ESP is in deep sleep mode
}