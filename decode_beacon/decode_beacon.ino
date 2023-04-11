//definations
String Manufacturing_Company = "Wildlife Acoustics, Inc.";
int beaconID_int;
byte beaconID_bits[8];
String telemetry_beacon;
String bird;
String mic2;
byte beacon_version[4];

byte Boxtype_and_serial[25];

String data = " manufacturer data: 30074110692786476294800003029000640069141600001f0000";


String beacon_byte[26];        // beacon bytes
byte binary_beacon_byte_2[8];  // binary values of beacon byte 2

int int_beacon_byte_234[3];

byte binary_beacon_byte_234[24];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Decode_beacon(data);
}

void loop() {
  // put your main code here, to run repeatedly:
}
void Decode_beacon(String input) {
  input.remove(0, 20);  // remove unncecessary data from received string
  for (int i = 0; i < 26; i++) {
    beacon_byte[i] = input.substring(i * 2, i * 2 + 2);
  }

  for (int i = 0; i < 26; i++) {  // print char array
    Serial.println(beacon_byte[i]);
  }

//****************maunfacturig company ****************
  if (beacon_byte[0] == "30" && beacon_byte[1] == "07")
    Serial.println(Manufacturing_Company);
  else {
    return;
  }

//*****************beacon id********************

  Serial.println("beacon ID");
  beaconID_int = beacon_byte[2].toInt();
  for (int j = 7; j > -1; j--) {  // convert int array to binary array
    beaconID_bits[j] = (beaconID_int >> j) & 0x01;
    //Serial.print(beaconID_bits[j]);
  }
  if (beaconID_bits[7] == 0) {
    telemetry_beacon = "tlemetry beacon";
  }
  if (beaconID_bits[6] == 0) {
    bird = "bird";
    Serial.println(bird);
  } else if (beaconID_bits[6] == 1) {
    bird = "bat";
    Serial.println(bird);
  }
  if (beaconID_bits[5] == 1) {
    mic2 = "mic 2 present";
    Serial.println(mic2);
  } else if (beaconID_bits[5] == 0) {
    mic2 = "mic 2 not present";
    Serial.println(mic2);
  }

  Serial.print("baecon version : ");
  for (int j = 4; j > 0; j--) {
    beacon_version[j] = beaconID_bits[j];
    Serial.print(beacon_version[j]);
  }

  //**************** box type*****************

    Serial.print("box type :");
  beaconID_int = beacon_byte[3].toInt();
  for (int j = 25; j > -1; j--) {  // convert int array to binary array
    beaconID_bits[j] = (beacon_byte[3].toInt() >> j) & 0x01;
    
  }

   for (int i = 4; i < 6; i++) {
}
//   for (int i = 4; i < 6; i++) {

//******************* box type ****************

//   for (int i = 4; i < 6; i++) {
//     int_beacon_byte_byte_234[i] = beacon_byte[i].toInt();  // convert str array to int array
//     //Serial.println(int_beacon_byte_byte_234[i]);
//     for (int j = 0; j < 8; j++) {// convert int array to binary array
//       binaryArray_byte_234[((i-2) * 8) + j] = (int_beacon_byte_byte_234[i] >> j) & 0x01;
//     }
//   }
//   for (int i = 0; i < 24; i++) {  // print binary array for next 3 bytes
//     Serial.print(binaryArray_byte_234[i]);
//   }

//   if (binaryArray_byte_234[0]==0)
// }