#include <WiFi.h>
#include <time.h>
#include <Wire.h>
#include "RTClib.h"

const char* ssid = "workplace";
const char* password = "devomech12121";
RTC_DS3231 rtc;
const char* ntpServer = "in.pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 18000;  //GMT+5:30
// RTC settin
const uint8_t alarmPin = 14; // GPIO14 is used as the RTC alarm trigger pin

void printTime() {
  DateTime now = rtc.now();
  Serial.printf("%02d:%02d:%02d\n", now.hour(), now.minute(), now.second());
}

void setup() {
  Serial.begin(115200);
  pinMode(alarmPin, INPUT_PULLUP);

  // Connect to Wi-Fi
  Serial.printf("Connecting to %s...", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");

  // Initialize the RTC module
  if (!rtc.begin()) {
    Serial.println("RTC not found");
    while (1);
  }
  rtc.disable32K(); // Disable the 32KHz output signal to save power
  rtc.writeSqwPinMode(DS3231_OFF); // Disable the square-wave output signal

  // Synchronize the RTC with the NTP server
  Serial.println("Synchronizing RTC with NTP server...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (!time(nullptr)) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("OK");

  // Set the RTC time
  rtc.adjust(DateTime(time(nullptr)));

  // Set the RTC alarm to trigger every day at 6:00 AM
  rtc.clearAlarm(1);
  rtc.setAlarm1(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day()+1, 6, 0, 0), DS3231_A1_Hour);

  Serial.println("Alarm set for 6:00 AM");

  // Go to deep sleep mode
  Serial.println("Going to deep sleep...");
  Serial.flush();
  esp_sleep_enable_ext0_wakeup((gpio_num_t)alarmPin, LOW); // Set the RTC alarm pin as the wakeup source
  esp_sleep_enable_timer_wakeup(30 * 1000000); // Wake up after 30 seconds if RTC alarm fails
  esp_deep_sleep_start();
}

void loop() {
  // This code should never be executed
}
