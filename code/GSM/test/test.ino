// Original code: https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G/blob/master/examples/Arduino_NetworkTest/Arduino_NetworkTest.ino

#include "GSM.h"



void setup(){
  // Set console baud rate
  SerialMon.begin(115200);
  modemPowerOn();
  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);


}

void loop(){
 GSM_connection();
}
