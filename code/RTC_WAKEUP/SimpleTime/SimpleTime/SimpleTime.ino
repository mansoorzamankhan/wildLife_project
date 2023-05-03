#include <WiFi.h>
#include "time.h"
#include "sntp.h"

const char* ssid = "workplace";
const char* password = "devomech12121";

const char* ntpServer = "in.pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 18000;  //GMT+5:30


void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.println(&timeinfo);
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval* t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}
void Connect_wifi() {
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
}
void disconect_wifi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}
void setup() {
  Serial.begin(115200);

  // set notification call-back function
  sntp_set_time_sync_notification_cb(timeavailable);
    sntp_servermode_dhcp(1);  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Connect_wifi();
   printLocalTime();
   disconect_wifi();


}

void loop() {

  delay(5000);
  printLocalTime();  // it will take some time to sync time :)
}
