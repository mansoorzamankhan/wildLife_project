#ifndef __BLE_SCAN_H__
  #define __BLE_SCAN_H__
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#define MAC " Address: 24:9f:28:d4:04:36"
String address = "";
String rawData = "";
String data = "";
int scanTime = 5;  //In seconds
BLEScan* pBLEScan;
bool device_found = false;
String parts[6];  // array to store parsed parts

int commaIndex = 0;  // index of the last comma found
int partIndex = 0;   // index of the current part being parsed

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {


    //print devices that are scaned
    //rawData = advertisedDevice.toString().c_str();
    rawData = advertisedDevice.toString().c_str();  // extract the raw data from becon
    // loop through each character in the input string
    //Serial.println( rawData);
    for (int i = 0; i < rawData.length(); i++) {
      if (rawData.charAt(i) == ',') {                         // if a comma is found
        parts[partIndex] = rawData.substring(commaIndex, i);  // parse the current part
        commaIndex = i + 1;                                   // update the comma index
        partIndex++;                                          // move on to the next part
      }
    }
    // parse the last part (after the last comma)
    parts[partIndex] = rawData.substring(commaIndex);
    // print the parsed adress

    address = parts[1];
    //Serial.println(address);
    partIndex = 0;
    commaIndex = 0;

    //Serial.println(address);
    if (address == MAC) {  // compare the address
      device_found = true;
      Serial.println("SM MINI found: ");
      data = parts[2];
      Serial.println(data);
      partIndex = 0;
      commaIndex = 0;
    }
  }
};

#endif