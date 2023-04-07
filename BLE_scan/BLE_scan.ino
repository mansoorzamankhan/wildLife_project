/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#define MAC "0c:b8:15:c4:0a:be"
String address = "";
String rawData = "";
String data = "";
int scanTime = 5;  //In seconds
BLEScan* pBLEScan;
bool device_found = false;
String parts[4];  // array to store parsed parts

int commaIndex = 0;  // index of the last comma found
int partIndex = 0;   // index of the current part being parsed

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
   
   Serial.print("available devices: ");
    Serial.println(address);  //print devices that are scaned

    if (address == MAC) {
      device_found = true;
      Serial.println("SM MINI found: ");
      rawData = advertisedDevice.toString().c_str();  // extract the raw data from becon
      // loop through each character in the input string
      for (int i = 0; i < rawData.length(); i++) {
        if (rawData.charAt(i) == ',') {                         // if a comma is found
          parts[partIndex] = rawData.substring(commaIndex, i);  // parse the current part
          commaIndex = i + 1;                                   // update the comma index
          partIndex++;                                          // move on to the next part
        }
      }
      // parse the last part (after the last comma)
      parts[partIndex] = rawData.substring(commaIndex);
      // print the parsed parts

      data=parts[2];
      Serial.println(data);
      partIndex = 0;
      commaIndex = 0;

      
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  if (device_found == false) {  // if beacon device is found then stop scanning 
    for (byte i = 0; i < 1; i++) { // scan again if the device is found to receive the second payload 
      BLEScanResults foundDevices = pBLEScan->start(scanTime, false);// get data from found device 
      address = BLEDevice::getAddress().toString().c_str();  // find adresses of scaned devices
      Serial.println("Scan done!");
      pBLEScan->clearResults();  // delete results fromBLEScan buffer to release memory
      delay(2000);

    }
  } 
}
