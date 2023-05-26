
#include <TimeLib.h>
#include "decode_beacon.h"
#include "BLE_scan.h"
#include "MQTT_publisher.h"
#include "sleep.h"
void setup() {
  Serial.begin(115200);
  // MQTT publisher
  
  DECODE_BEACON();
  mqtt_Setup();
  connect_Wifi();
  connect_MQTT();
  MQTT_PUBLISH();
  set_RTC_and_sleep_time();
  go_deep_sleep();
}

void loop() {
}
void BLE_INIT(void) {
  // beacon Scan
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}
void BLE_SCAN(void) {
  // Beacon scan:
  if (device_found == false) {      // if beacon device is found then stop scanning
    for (byte i = 0; i < 1; i++) {  // scan again if the device is found to receive the second payload
      Serial.println("available devices: ");
      BLEScanResults foundDevices = pBLEScan->start(scanTime, false);  // get data from found device
      Serial.println("Scan done!");
      pBLEScan->clearResults();  // delete results fromBLEScan buffer to release memory
      delay(2000);
    }
  }
}
void DECODE_BEACON(void) {
  //decode beacon data
  Decode_telemetry(data_beacon1);
  Decode_recording(data_beacon2);
  recorder_name = prefix1 + prefix2;
  Serial.print("recorder name : ");
  Serial.println(recorder_name);
}
void MQTT_PUBLISH(void) {
  String MQTT_data = Manufacturing_Company +","+ bird +","+ mic2 +","+ box_type +","+ SD_capacity +","+ SD_used\
   +","+ Temprature +","+ battery_level +","+ Firmware_index +","+ error_code + ","+recording_no +","+ TimeZone \
   +","+current_time +","+ Time_of_next_recording +","+ current_schedule +","+ Sampling_rate +","+ recorder_name ;




send_MQTT_data(recorder_name, MQTT_data);
  // send_MQTT_data("bird", bird);
  // send_MQTT_data("mic2", mic2);
  // send_MQTT_data("box_type", box_type);
  // send_MQTT_data("SD_capacity", SD_capacity);
  // send_MQTT_data("SD_used", SD_used);
  // send_MQTT_data("Temprature", Temprature);
  // send_MQTT_data("battery_level", battery_level);
  // send_MQTT_data("Firmware_index", Firmware_index);
  // send_MQTT_data("error_code", error_code);
  // send_MQTT_data("recording_no", recording_no);
  // send_MQTT_data("TimeZone", TimeZone);
  // send_MQTT_data("current_time", current_time);
  // send_MQTT_data("Time_of_next_recording", Time_of_next_recording);
  // send_MQTT_data("length_of_next_recording", length_of_next_recording);
  // send_MQTT_data("current_schedule", current_schedule);
  // send_MQTT_data("Sampling_rate", Sampling_rate);
  // send_MQTT_data("recorder_name", recorder_name);
}
