
#include <TimeLib.h>
#include "decode_beacon.h"
#include "BLE_scan.h"

void setup() {

  Serial.begin(115200);

  // beacon Scan 
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

}

void loop() {


}

void decode_Data(void)
{
    //decode beacon data 
  Decode_telemetry(data_beacon1);
  Decode_recording(data_beacon2);
  recorder_name= prefix1+prefix2;
  Serial.print("recorder name : ");
  Serial.print(recorder_name);
}
void Beacon_scan(void)
{
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
