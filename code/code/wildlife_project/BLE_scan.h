#ifndef __BLE_SCAN_H__
  #define __BLE_SCAN_H__
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#define MAC " Address: 9c:25:be:10:69:27"
String address = "";
String raw_data = "";
String _data = "";
String payload_data[6];
int scanTime = 5;  //In seconds
BLEScan* pBLEScan;
bool device_found = false;
String parts[6];  // array to store parsed parts

int commaIndex = 0;  // index of the last comma found
int partIndex = 0;   // index of the current part being parsed

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {


    //print devices that are scaned
    //raw_data = advertisedDevice.toString().c_str();
    raw_data = advertisedDevice.toString().c_str();  // extract the raw _data from becon
    // loop through each character in the input string
    //Serial.println( raw_data);
    for (int i = 0; i < raw_data.length(); i++) {
      if (raw_data.charAt(i) == ',') {                         // if a comma is found
        parts[partIndex] = raw_data.substring(commaIndex, i);  // parse the current part
        commaIndex = i + 1;                                   // update the comma index
        partIndex++;                                          // move on to the next part
      }
    }
    // parse the last part (after the last comma)
    parts[partIndex] = raw_data.substring(commaIndex);
    // print the parsed adress

    address = parts[1];
    //Serial.println(address);
    partIndex = 0;
    commaIndex = 0;

    //Serial.println(address);
    if (address == MAC) {  // compare the address
      device_found = true;
      Serial.println("SM MINI found: fetching payload ");
      _data = parts[2];
     // Serial.println(_data);
      partIndex = 0;
      commaIndex = 0;
    }
  }
};

#endif