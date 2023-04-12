//definations
String Manufacturing_Company = "Wildlife Acoustics, Inc.";
//beacon id
int beaconID_int;
byte output_bits[8];
String telemetry_beacon;
String bird;
String mic2;
String box_type;
byte beacon_version[4];
//box and serial
String SD_capacity;
String SD_used;
String Tempraure;
String battery_level;
String Firlware_index;
String error_code;
String recording_no;
String data = " manufacturer data: 30074110692786476294800003029000640069141600001f0000";


String beacon_byte[26];        // beacon bytes
byte binary_beacon_byte_2[8];  // binary values of beacon byte 2

int int_beacon_byte_234[3];

byte binary_beacon_byte_234[24];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Decode_telemetry(data);
}

void loop() {
  // put your main code here, to run repeatedly:
}
void Decode_telemetry(String input) {
  input.remove(0, 20);  // remove unncecessary data from received string
  for (int i = 0; i < 26; i++) {
    beacon_byte[i] = input.substring(i * 2, i * 2 + 2);
  }

  for (int i = 0; i < 26; i++) {  // print char array
    Serial.print("[");
    Serial.print(beacon_byte[i]);
    Serial.print("]");
  }
  Serial.println("");
  //****************maunfacturig company ****************
  //Serial.println("byte 0,1 (manufacturer) ");
  if (beacon_byte[0] == "30" && beacon_byte[1] == "07")
    Serial.println(Manufacturing_Company);
  else {
    return;
  }

  //*****************beacon id********************

   byte_to_bit(beacon_byte[2]);
  display_bits();

  if (output_bits[7] == 0) {
    telemetry_beacon = "tlemetry beacon";
    Serial.print("paylaod type : ");
    Serial.println(telemetry_beacon);
  }
  if (output_bits[6] == 0) {
    bird = "bird";
    Serial.print("bird type : ");
    Serial.println(bird);
  } else if (output_bits[6] == 1) {
    bird = "bat";
    Serial.print("bird type : ");
    Serial.println(bird);
  }
  if (output_bits[5] == 1) {
    mic2 = " present";
    Serial.print("secondary mic  : ");
    Serial.println(mic2);
  } else if (output_bits[5] == 0) {
    mic2 = "absent";
    Serial.print("secondary mic  : ");
    Serial.println(mic2);
  }

  Serial.print("baecon version : ");
  for (int i = 0; i < 4; i++) {
    beacon_version[i] = output_bits[i];
    Serial.print(beacon_version[i]);
  }
  Serial.println("");

  //**************** box type*****************

  //Serial.println("byte 3 ( box type)");
  byte_to_bit(beacon_byte[3]);
  //display_bits();

  if (output_bits[3] == 1) {
    box_type = "Bat box";
    Serial.print("box_type : ");
    Serial.println(box_type);
  } else if (output_bits[3] == 0) {
    box_type = "bird box";
    Serial.print("box_type");
    Serial.println(box_type);
  }
  //*************** Sd card capacity **************
  SD_capacity = beacon_byte[13] + beacon_byte[12];
  Serial.print("Sd card capacity: ");
  Serial.println(SD_capacity);

  //*************** Sd card used     **************
  SD_used = beacon_byte[15] + beacon_byte[14];
  Serial.print("Sd card used: ");
  Serial.println(SD_used);
  //*************** Temprature     **************
  Tempraure = beacon_byte[17] + beacon_byte[16];
  Serial.print("Temprature: ");
  Serial.println(Tempraure);
  //*************** battery level **************
  battery_level = beacon_byte[19] + beacon_byte[18];
  Serial.print("Battery level: ");
  Serial.println(battery_level);
  //***************firmware index******************
  Firlware_index = beacon_byte[20] ;
  Serial.print("Firlware : ");
  Serial.println(Firlware_index);
  //**************** Error code *******************
  error_code =beacon_byte[22] + beacon_byte[21];
  Serial.print("error code : ");
  Serial.println(error_code);
  if (error_code=="0000") error_code="no error";
  if (error_code=="0001") error_code="no date/time received ";
  if (error_code=="0002") error_code="STM RTC battery missing/low";
  if (error_code=="0003") error_code="SD Card missing";
  if (error_code=="0004") error_code="SD Card full";
  if (error_code=="0005") error_code="Last SD Card Failed.";
  if (error_code=="0006") error_code="Received STM32 firmware is corrupt";
  if (error_code=="0007") error_code="Received BT module firmware is corrupt";
  if (error_code=="0008") error_code="Update of STM32 firmware failed";
  if (error_code=="0009") error_code="Update of BT module firmware failed";
  if (error_code=="000A") error_code="Something in the configuration doesn't make sense";
  if (error_code=="000B") error_code="schedule doesn't make sense";
  if (error_code=="000C") error_code="There's no microphone.";
    //**************** number of recordings*******************
  recording_no =beacon_byte[25] + beacon_byte[24]+ beacon_byte[23];
  Serial.print("recording_no : ");
  Serial.println(recording_no);
}



void byte_to_bit(String input) {
  for (int i = 0; i < 8; i++) {
    output_bits[i] = (input.toInt() >> i) & 0x01;
  }
}
void display_bits(void) {
  for (int i = 7; i > -1; i--) {
    Serial.print(output_bits[i]);
  }
  Serial.println("");
}
