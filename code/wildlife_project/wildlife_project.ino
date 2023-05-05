
#include <EEPROM.h>
int flag_address = 0;  // address in the EEPROM to write to
bool published_flag = LOW ;
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
  BLE_INIT();
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
  set_RTC_and_sleep_time();
  EEPROM.get(flag_address,published_flag);
  if (((int)hour() >= wakeup_hour && (int)minute() > wakeup_minute) &&  published_flag== LOW) {
    connect_Wifi();
    connect_MQTT();
    MQTT_PUBLISH();
    published_flag = HIGH;
    save_flag(published_flag);
  } else {
    wakeup_reason();
    
    go_deep_sleep();
    published_flag = LOW;
    save_flag(published_flag);
  }
  //Connect_GSM();
}

void BLE_SCAN(void) {
  // Beacon scan:
  if (device_found == false) {                                         // if beacon device is found then stop scanning
    for (byte i = 0; i < 6; i++) {                                     // scan again if the device is found to receive the second payload
      BLEScanResults foundDevices = pBLEScan->start(scanTime, false);  // get data from found device

      payload_data[i] = _data;
      pBLEScan->clearResults();  // delete results fromBLEScan buffer to release memory
      delay(2000);
      //Serial.println(payload_data[i]);
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
void save_flag(bool value) {
  EEPROM.begin(EEPROM.length());  // initialize the EEPROM
  EEPROM.put(flag_address, value);  // write the value to the EEPROM
  EEPROM.commit();             // save the changes
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
