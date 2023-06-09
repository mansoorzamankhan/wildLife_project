
#include <EEPROM.h>
#define flag_address 0  // address in the EEPROM to write to
#define day_address 4
bool published_flag = false;
uint8_t dayOfPosting = 0;
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
  Serial.print("published flag status ");
  Serial.println(published_flag);

  dayOfPosting = EEPROM.read(day_address);  // det data from eeprom for day of last posting
  Serial.print("last day when data was posted  ");
  Serial.println(dayOfPosting);
  if (dayOfPosting != current_day) {  // if its not  the same day after posting then lower the flag to publish data again
    published_flag = false;

  } else {  // if its same day them
    Serial.println("data was already posted today ");
  }

  //if time passes from 6AM  and no data is pubished yet then publish data
  if ((current_hour >= wakeup_hour && current_minute >= wakeup_minute) && published_flag == false) {
    Serial.println("its posting time, checking flag status..");
    Serial.print("published flag status ");
    Serial.println(published_flag);
    connect_Wifi();
    connect_MQTT();
    MQTT_PUBLISH();
    set_RTC_and_sleep_time();
    // set the published flag and save it to eeprom
    published_flag = true;
    EEPROM.write(flag_address, published_flag);
    EEPROM.commit();
    //save the date in eeprom
    dayOfPosting = current_day;
    EEPROM.write(day_address, dayOfPosting);
    EEPROM.commit();
    go_deep_sleep();

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
  String MQTT_data = manufacturing_company + "," + bird + "," + mic2 + "," + box_type + "," + SD_capacity + "," + SD_used
                     + "," + temperature + "," + battery_level + "," + firmware_index + "," + error_code + "," + recording_no + "," + time_zone
                     + "," + current_time + "," + time_of_next_recording + "," + current_schedule + "," + sampling_rate + "," + recorder_name;

  send_MQTT_data(recorder_name, MQTT_data);
}
