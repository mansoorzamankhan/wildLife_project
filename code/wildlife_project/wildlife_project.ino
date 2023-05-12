
#include <EEPROM.h>
#define flag_address 0  // address in the EEPROM to write to
bool published_flag = false;
#include <TimeLib.h>
#include "decode_beacon.h"
#include "BLE_scan.h"
#include "sleep.h"
//#include "MQTT_publisher_GSM.h"
#include "MQTT_publisher_WIFI.h"
bool telemetry_detected = false;
bool recording_detected = false;
void setup() {
  Serial.begin(115200);
  // Initialize the EEPROM with a size of 512 bytes
  EEPROM.begin(512);
  //BLE initialization
  BLE_INIT();
  //MQTT setup initialization
  mqtt_Setup();
}

void loop() {
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n');
    if (inputString == "restart") {
      ESP.restart();
    }
  }
  BLE_SCAN();
  DECODE_BEACON();
  Serial.println("*********************************************");
  published_flag = EEPROM.read(flag_address);  //get data from eepronm for published flag
  Serial.print("flag status ");
  Serial.println(published_flag);
  //if time passes from 6AM  and no data is pubished yet then publish data

  if ((current_hour >= wakeup_hour && current_minute >= wakeup_minute) && published_flag == false) {
    Serial.println("posting time, checking flag status..");
    Serial.print("flag status ");
    Serial.println(published_flag);
    connect_Wifi();
    connect_MQTT();
    MQTT_PUBLISH();
    set_RTC_and_sleep_time();
    published_flag = true;
    EEPROM.write(flag_address, published_flag);
    EEPROM.commit();
    go_deep_sleep();
    // after waking up from sleep
    published_flag = false;  // set the published flag low inorder to publish the data again
    EEPROM.write(flag_address, published_flag);
    EEPROM.commit();

  } else {
    Serial.println("going to sleep without posting  ");
    set_RTC_and_sleep_time();
    
    go_deep_sleep();
    wakeup_reason();
    // after waking up from sleep
    published_flag = false;  // set the published flag low inorder to publish the data again
    EEPROM.write(flag_address, published_flag);
    EEPROM.commit();
  }
  //Connect_GSM();
}

void BLE_SCAN(void) {
  // Beacon scan:
  while (device_found == false) {  // if beacon device is found then stop scanning
    Serial.print("Scanning beacon ...");
    for (byte i = 0; i < 6; i++) {                                     // scan again if the device is found to receive the second payload
      BLEScanResults foundDevices = pBLEScan->start(scanTime, false);  // get data from found device

      payload_data[i] = _data;
      pBLEScan->clearResults();  // delete results fromBLEScan buffer to release memory
      delay(2000);
      Serial.print(".");
    }
    Serial.println("Scan done!");
  }
}
void DECODE_BEACON(void) {
  //decode beacon data
  for (byte i = 0; i < 6; i++) {
    if (telemetry_detected == true && recording_detected == true) {
      break;
    }
    Decode_type(payload_data[i]);
    if (payload_type == "Telemetry_payload" && !telemetry_detected) {
      Decode_telemetry(payload_data[i]);
      telemetry_detected = true;
    } else if (payload_type == "Recording_payload" && !recording_detected) {
      Decode_recording(payload_data[i]);
      recording_detected = true;
    }
  }




  recorder_name = prefix1 + prefix2;
  Serial.print("recorder name : ");
  Serial.println(recorder_name);
}

void MQTT_PUBLISH(void) {
  send_MQTT_data("manufacturing_company", manufacturing_company);
  send_MQTT_data("bird", bird);
  send_MQTT_data("mic2", mic2);
  send_MQTT_data("box_type", box_type);
  send_MQTT_data("SD_capacity", SD_capacity);
  send_MQTT_data("SD_used", SD_used);
  send_MQTT_data("temperature", temperature);
  send_MQTT_data("battery_level", battery_level);
  send_MQTT_data("firmware_index", firmware_index);
  send_MQTT_data("error_code", error_code);
  send_MQTT_data("recording_no", recording_no);
  send_MQTT_data("time_zone", time_zone);
  send_MQTT_data("current_time", current_time);
  send_MQTT_data("time_of_next_recording", time_of_next_recording);
  send_MQTT_data("length_of_next_recording", length_of_next_recording);
  send_MQTT_data("current_schedule", current_schedule);
  send_MQTT_data("Sampling_rate", Sampling_rate);
  send_MQTT_data("recorder_name", recorder_name);
}
