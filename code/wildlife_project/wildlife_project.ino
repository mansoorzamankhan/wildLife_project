
#include <TimeLib.h>
#include "decode_beacon.h"
#include "BLE_scan.h"
#include "MQTT_publisher.h"
bool telemetry_detected = false;
bool recording_detected = false;
void setup() {
  Serial.begin(115200);
  BLE_INIT();
  BLE_SCAN();
  // MQTT publisher
  DECODE_BEACON();
  mqtt_Setup();
  Connect_GSM();
  connect_MQTT();
  MQTT_PUBLISH();
}

void loop() {
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n');
    if (inputString == "restart") {
      ESP.restart();
    }
  }
}
void BLE_INIT(void) {
  // beacon Scan
  Serial.println("Scanning beacon ...");
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
    for (byte i = 0; i < 6; i++) {  // scan again if the device is found to receive the second payload
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
