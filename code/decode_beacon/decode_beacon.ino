#include <TimeLib.h>

//definations
String Manufacturing_Company = "Wildlife Acoustics, Inc.";
//beacon id
String TIME;
int beaconID_int;
unsigned int intValue;
byte output_bits[8];
String payload_type;
String bird;
String mic2;
String box_type;
byte beacon_version[4];
//box and serial
String SD_capacity;
String SD_used;
String Temprature;
String battery_level;
String Firmware_index;
String error_code;
String recording_no;
String TimeZone;
String current_time;
String Time_of_next_recording;
String length_of_next_recording;
String current_schedule;
String Sampling_rate;
String data = " manufacturer data: 30074110692786476294800003029000640069141600001f0000";
//String data = " manufacturer data: 3007c110692705095b70000004447cc62bd871c62b183102010b";


String beacon_byte[26];        // beacon bytes
byte binary_beacon_byte_2[8];  // binary values of beacon byte 2

int int_beacon_byte_234[3];

byte binary_beacon_byte_234[24];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Decode_telemetry(data);
  //Decode_recording(data);
}

void loop() {
  // put your main code here, to run repeatedly:
}
void Decode_recording(String input) {
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
  // **************** manufacturer**************
  if (beacon_byte[0] == "30" && beacon_byte[1] == "07")
    Serial.println(Manufacturing_Company);
  else {
    return;
  }
  //*******************payload type *************
  hex_to_int(beacon_byte[2]);
  int_to_bits();
  //display_bits();
  if (output_bits[7] == 1) {
    payload_type = "Recording payload";
    Serial.print("paylaod type : ");
    Serial.println(payload_type);
  }


  //******************** time zone *******************
  hex_to_int(beacon_byte[12]);
  TimeZone = String(intValue * 0.25);
  Serial.print("Time zone:UTC +");
  Serial.println(TimeZone);
  //******************* current time *****************
  current_time = beacon_byte[16] + beacon_byte[15] + beacon_byte[14] + beacon_byte[13];
  hex_to_int(current_time);
  time_conversion(intValue);
  current_time = TIME;
  Serial.print("Current time : ");
  Serial.println(current_time);

  //******************** time of next recording************
  Time_of_next_recording = beacon_byte[20] + beacon_byte[19] + beacon_byte[18] + beacon_byte[17];
  hex_to_int(Time_of_next_recording);
  time_conversion(intValue);
  Time_of_next_recording = TIME;
  Serial.print("Time of next recording : ");
  Serial.println(Time_of_next_recording);

  //***********length of next recording *****************
  length_of_next_recording = beacon_byte[23] + beacon_byte[22] + beacon_byte[21];
  hex_to_int(length_of_next_recording);
  length_of_next_recording = String(intValue);
  Serial.print("length of next recording : ");
  Serial.println(length_of_next_recording);

  //************************current schedule+++++++++


  current_schedule = String(beacon_byte[24]);


  if (current_schedule == "00") current_schedule = "Custom Schedule";
  if (current_schedule == "01") current_schedule = "24 hour recording";
  if (current_schedule == "02") current_schedule = "Sunrise to sunset recording. ";
  if (current_schedule == "03") current_schedule = "30min before sunset to 30 min after sunrise ";
  if (current_schedule == "04") current_schedule = "24 hour recording";
  if (current_schedule == "05") current_schedule = "20min each hour ";
  if (current_schedule == "06") current_schedule = "5min each hour ";
  if (current_schedule == "07") current_schedule = "Sunrise to suneset  ";
  if (current_schedule == "08") current_schedule = "2hr before sunrise to 2hrs after sunset ";
  Serial.print("current_schedule : ");
  Serial.println(current_schedule);


  //************************current schedule+++++++++


  Sampling_rate = String(beacon_byte[25]);



  if (Sampling_rate == "01") Sampling_rate = "8000";
  if (Sampling_rate == "02") Sampling_rate = "12000";
  if (Sampling_rate == "03") Sampling_rate = "16000 ";
  if (Sampling_rate == "04") Sampling_rate = "22050";
  if (Sampling_rate == "06") Sampling_rate = "32000";
  if (Sampling_rate == "07") Sampling_rate = "44100";
  if (Sampling_rate == "08") Sampling_rate = "48000 ";
  if (Sampling_rate == "09") Sampling_rate = "64000 ";
  if (Sampling_rate == "0a") Sampling_rate = "88200 ";
  if (Sampling_rate == "0b") Sampling_rate = "96000 ";
  if (Sampling_rate == "0c") Sampling_rate = "256000 ";
  if (Sampling_rate == "0d") Sampling_rate = "384000 ";
  if (Sampling_rate == "0e") Sampling_rate = "500000";

  Serial.print("Sampling_rate : ");
  Serial.println(Sampling_rate);
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
  hex_to_int(beacon_byte[2]);
  int_to_bits();
  display_bits();

  if (output_bits[7] == 0) {
    payload_type = "tlemetry payload";
    Serial.print("paylaod type : ");
    Serial.println(payload_type);
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
  hex_to_int(beacon_byte[3]);
  int_to_bits();
  display_bits();

  if (output_bits[4] == 1) {
    box_type = "Bat box ";
    Serial.print("box_type : ");
    Serial.println(box_type);
  } else if (output_bits[4] == 0) {
    box_type = " bird box";
    Serial.print("box_type");
    Serial.println(box_type);
  }
  //*************** Sd card capacity **************
  SD_capacity = beacon_byte[13] + beacon_byte[12];
  hex_to_int(SD_capacity);
  SD_capacity = String(intValue * 0.0625);
  Serial.print("Sd card capacity: ");
  Serial.println(SD_capacity);

  //*************** Sd card used     **************
  SD_used = beacon_byte[15] + beacon_byte[14];
  hex_to_int(SD_used);
  SD_used = String(intValue * 0.0625);
  Serial.print("Sd card used: ");
  Serial.println(SD_used);
  //*************** Temprature     **************
  Temprature = beacon_byte[17] + beacon_byte[16];
  hex_to_int(Temprature);
  Temprature = String(intValue * 0.25);
  Serial.print("Temprature: ");
  Serial.println(Temprature);
  //*************** battery level **************
  battery_level = beacon_byte[19] + beacon_byte[18];
  hex_to_int(battery_level);
  battery_level = String(intValue * 0.001);
  Serial.print("Battery level: ");
  Serial.println(battery_level);
  //***************firmware index******************
  Firmware_index = beacon_byte[20];
  hex_to_int(Firmware_index);
  Firmware_index = String(intValue);
  Serial.print("Firlware : ");
  Serial.println(Firmware_index);
  //**************** Error code *******************
  error_code = beacon_byte[22] + beacon_byte[21];
  Serial.print("error code : ");
  Serial.println(error_code);
  if (error_code == "0000") error_code = "no error";
  if (error_code == "0001") error_code = "no date/time received ";
  if (error_code == "0002") error_code = "STM RTC battery missing/low";
  if (error_code == "0003") error_code = "SD Card missing";
  if (error_code == "0004") error_code = "SD Card full";
  if (error_code == "0005") error_code = "Last SD Card Failed.";
  if (error_code == "0006") error_code = "Received STM32 firmware is corrupt";
  if (error_code == "0007") error_code = "Received BT module firmware is corrupt";
  if (error_code == "0008") error_code = "Update of STM32 firmware failed";
  if (error_code == "0009") error_code = "Update of BT module firmware failed";
  if (error_code == "000A") error_code = "Something in the configuration doesn't make sense";
  if (error_code == "000B") error_code = "schedule doesn't make sense";
  if (error_code == "000C") error_code = "There's no microphone.";

  //**************** number of recordings*******************
  recording_no = beacon_byte[25] + beacon_byte[24] + beacon_byte[23];
  hex_to_int(recording_no);
  recording_no = String(intValue);
  Serial.print("recording_no : ");
  Serial.println(recording_no);
}

void hex_to_int(String input) {
  intValue = strtol(input.c_str(), NULL, 16);
}
void int_to_bits(void) {
  for (int i = 0; i < 8; i++) {
    output_bits[i] = (intValue >> i) & 0x01;
  }
}
void time_conversion(unsigned int _time) {
  setTime(946684800);
  adjustTime(_time);
  TIME = String(day()) + "/" + String(month()) + "/" + String(year()) + "  " + String(hour()) + ":" + String(minute()) + ":" + String(second());
}


void display_bits(void) {
  for (int i = 7; i > -1; i--) {
    Serial.print(output_bits[i]);
  }
  Serial.println("");
}
