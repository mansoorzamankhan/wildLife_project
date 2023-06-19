#ifndef __DECODE_BEACON_H__
#define __DECODE_BEACON_H__
int current_hour ;
int current_minute ;
int current_second;
int current_day;
int current_month;
int current_year;
//definations
String manufacturing_company = "Wildlife Acoustics, Inc.";
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
String temperature;
String battery_level;
String firmware_index;
String error_code;
String recording_no;
String time_zone;
String current_time;
String time_of_next_recording;
String length_of_next_recording;
String current_schedule;
String sampling_rate;
String prefix1;
String prefix2;
String recorder_name;
// String data_beacon1 = " manufacturer data: 30074110692786476294800003029000640069141600001f0000";
// String data_beacon2 = " manufacturer data: 3007c110692705095b70000004447cc62bd871c62b183102010b";
const char lut[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";

String beacon_byte[26];        // beacon bytes
byte binary_beacon_byte_2[8];  // binary values of beacon byte 2

int int_beacon_byte_234[3];

byte binary_beacon_byte_234[24];


void display_bits(void);
void time_conversion(unsigned int _time);
void int_to_bits(void);
void hex_to_int(String input);
void Decode_telemetry(String input);
void Decode_recording(String input);
void Decode_type(String payload);

void Decode_type(String payload) {

  void hex_to_int(String payload);
  void int_to_bits(void);
  void time_conversion(unsigned int _time);
  void display_bits(void);

  payload.remove(0, 20);  // remove unncecessary data from received string
  for (int i = 0; i < 26; i++) {
    beacon_byte[i] = payload.substring(i * 2, i * 2 + 2);
  }
  for (int i = 0; i < 26; i++) {  // print char array
    Serial.print("[");
    Serial.print(beacon_byte[i]);
    Serial.print("]");
  }
  Serial.println("");
  // **************** manufacturer**************
  if (beacon_byte[0] == "30" && beacon_byte[1] == "07")
    Serial.println(manufacturing_company);
  else {
    return;
  }
  //*******************payload type *************
  hex_to_int(beacon_byte[2]);
  int_to_bits();
  //display_bits();
  if (output_bits[7] == 1) {
    payload_type = "Recording_payload";
    Serial.print("paylaod type : ");
    Serial.println(payload_type);
  } else if (output_bits[7] == 0) {
    payload_type = "Telemetry_payload";
    Serial.print("paylaod type : ");
    Serial.println(payload_type);
  }
}

void Decode_recording(String input) {
  //******************** time zone *******************
  hex_to_int(beacon_byte[12]);
  time_zone = String(intValue * 0.25);
  time_zone = "UTC +" + time_zone;
  Serial.print("Time zone:UTC +");
  Serial.println(time_zone);
  //******************* current time *****************
  current_time = beacon_byte[16] + beacon_byte[15] + beacon_byte[14] + beacon_byte[13];
  hex_to_int(current_time);
  time_conversion(intValue);
  current_time = TIME;
  current_hour = String(hour()).toInt();
  current_minute = String(minute()).toInt();
  current_second = String(second()).toInt();
  current_day = String(day()).toInt();
  current_month = String(month()).toInt();
  current_year = String(year()).toInt();
  Serial.print("Current time : ");
  Serial.println(current_time);

  //******************** time of next recording************
  time_of_next_recording = beacon_byte[20] + beacon_byte[19] + beacon_byte[18] + beacon_byte[17];
  hex_to_int(time_of_next_recording);
  time_conversion(intValue);
  time_of_next_recording = TIME;
  Serial.print("Time of next recording : ");
  Serial.println(time_of_next_recording);

  //***********length of next recording *****************
  length_of_next_recording = beacon_byte[23] + beacon_byte[22] + beacon_byte[21];
  hex_to_int(length_of_next_recording);
  length_of_next_recording = String(intValue);
  Serial.print("length of next recording : ");
  Serial.println(length_of_next_recording);



  //******************* prefix2 ************************
  int intArray_recording[] = {};
  byte _bits_recording[40];
  for (byte i = 0; i < 5; i++) {
    hex_to_int(beacon_byte[i + 6]);
    intArray_recording[i] = intValue;

    byte k = 7;
    for (byte j = 0; j < 8; j++) {
      int_to_bits();

      _bits_recording[(i * 8) + j] = output_bits[k];
      k--;
      //Serial.print(_bits[(i * 8) + j]);
    }
  }


  for (int i = 0; i < 36; i += 6) {
    byte sixBits_recording = 0;
    for (int j = 0; j < 6; j++) {
      sixBits_recording <<= 1;
      sixBits_recording |= _bits_recording[i + j];
      //Serial.println(sixBits_telemetry);
    }
    prefix2 += lut[sixBits_recording];
  }
  Serial.print("prefix  2: ");
  Serial.println(prefix2);

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


  sampling_rate = String(beacon_byte[25]);



  if (sampling_rate == "01") sampling_rate = "8000";
  if (sampling_rate == "02") sampling_rate = "12000";
  if (sampling_rate == "03") sampling_rate = "16000 ";
  if (sampling_rate == "04") sampling_rate = "22050";
  if (sampling_rate == "06") sampling_rate = "32000";
  if (sampling_rate == "07") sampling_rate = "44100";
  if (sampling_rate == "08") sampling_rate = "48000 ";
  if (sampling_rate == "09") sampling_rate = "64000 ";
  if (sampling_rate == "0a") sampling_rate = "88200 ";
  if (sampling_rate == "0b") sampling_rate = "96000 ";
  if (sampling_rate == "0c") sampling_rate = "256000 ";
  if (sampling_rate == "0d") sampling_rate = "384000 ";
  if (sampling_rate == "0e") sampling_rate = "500000";

  Serial.print("sampling_rate : ");
  Serial.println(sampling_rate);
}
void Decode_telemetry(String input) {

  //*****************beacon id********************
  hex_to_int(beacon_byte[2]);
  int_to_bits();
  //display_bits();

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
  //display_bits();

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
  //*************** temperature     **************
  temperature = beacon_byte[17] + beacon_byte[16];
  hex_to_int(temperature);
  temperature = String(intValue * 0.25);
  Serial.print("temperature: ");
  Serial.println(temperature);
  //*************** battery level **************
  battery_level = beacon_byte[19] + beacon_byte[18];
  hex_to_int(battery_level);
  battery_level = String(intValue * 0.001);
  Serial.print("Battery level: ");
  Serial.println(battery_level);
  //***************firmware index******************
  firmware_index = beacon_byte[20];
  hex_to_int(firmware_index);
  firmware_index = String(intValue);
  Serial.print("Firlware : ");
  Serial.println(firmware_index);
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


  //******************* prefix ********************

  int intArray_telemetry[] = {};
  byte _bits_telemetry[40];
  for (byte i = 0; i < 5; i++) {
    hex_to_int(beacon_byte[i + 6]);
    intArray_telemetry[i] = intValue;

    byte k = 7;
    for (byte j = 0; j < 8; j++) {
      int_to_bits();

      _bits_telemetry[(i * 8) + j] = output_bits[k];
      k--;
    }
  }
  for (int i = 0; i < 36; i += 6) {
    byte sixBits_telemetry = 0;
    for (int j = 0; j < 6; j++) {
      sixBits_telemetry <<= 1;
      sixBits_telemetry |= _bits_telemetry[i + j];
    }
    prefix1 += lut[sixBits_telemetry];
  }
  Serial.print("prefix1 : ");
  Serial.println(prefix1);
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


#endif
