#include "GSM.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  GPRS_wake_up();
  Serial.print("GSM Status  : " );
  Serial.println(GPRS_connectivity_status());
}

void loop() {
  // put your main code here, to run repeatedly:
}
