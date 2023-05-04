#include "sleep.h"
void setup() {
  Serial.begin(115200);
}
void loop() {

  wakeup_reason();
  set_RTC_and_sleep_time();
  go_deep_sleep();
}