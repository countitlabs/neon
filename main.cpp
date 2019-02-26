// File: main.cpp
// Description: This is the main file for the COUNT.IT NEON IOT project
// Arduino Libraries needed: ArduinoJson Version: 5.13.2,  WifiNina Version: 1.3.0 and elapsedMilis v 1.0.4
// Note: This code is specific for Arduino Uno Wifi Rev2
// Author: Keith Low

#include "SPI.h";

#include "api.h";
#include "light_control.h";
#include "pwm.h";
#include "control_sequence.h";
#include "access_point.h";
#include "eeprom_data.h";

#include "config.h";



//#################################
//###### GLOBAL OBJECTS ###########
//#################################

AccessPoint ap(Config_CountItHotSpotName, Config_CountItHotSpotPassword , Config_CountItHotSpotTimer); //We want 5 minutes of timer which is = 300000 miliseconds

Api CountItRequest("","",""); //Object constructor is empty to be global - Check attach() function for constructor functionality

Data eeprom_data("","","");

ControlSequence Sequence;

//#################################
//#### END OF GLOBAL OBJECTS ######
//#################################

// -----------------------------------------------------------------

//#################################
//########GLOBAL VARIBLES:#########
//#################################

//********************************
//LIGHT CONTROL
int* temp_pins;
bool isSame = true;

//********************************
//ACCESS-POINT
String groupID;
String network;
String password;

//********************************
//EEPROM DATA
int data_size;
bool checkTime;

String group;
String wifi;
String pass;
String new_group;
String new_wifi;
String new_pass;
//********************************

//#################################
//#### END OF GLOBAL VARIBLES:#####
//#################################

// -----------------------------------------------------------------

void setup(){
  Serial.begin(9600);
  Serial.println("Starting network functions");

  data_size = eeprom_data.found_data();
  Serial.println(data_size);

  //The addition of all delimeters is = 4 
  //so if the len of eeprom_data is more than 5 then we have some data in it.

  if (data_size > 5) 
  {
    Serial.println("There is data!");
    
    eeprom_data.load_data(data_size);
    delay(2000);

    group = eeprom_data.get_group_id();
    wifi = eeprom_data.get_wifi_name();
    pass = eeprom_data.get_wifi_password();

    Serial.print("Group id eeprom: ");
    Serial.println(group);
    Serial.print("Wifi name eeprom: ");
    Serial.println(wifi);
    Serial.print("Password eeprom: ");
    Serial.println(pass);

    ap.startAP();
    checkTime = ap.checkAPWithTimer();

    if (checkTime){
      Serial.println("Timer is done!");
      Serial.println("directly connecting to wifi!");
      ap.connectDirectlyToWifi(wifi,pass);
      new_group = group;
    } 
    else if (!checkTime) {
      //If checktime is false - Means the user inputted something
      //So we have to save that new value
      Serial.println("User inputted something");

      groupID = ap.get_groupId();
      password = ap.get_password();
      network = ap.get_wifiName();

      eeprom_data.attach(groupID,network,password);
      eeprom_data.save_data();
      data_size = eeprom_data.found_data(); // Gets the new data size 
      eeprom_data.load_data(data_size);

      new_group = eeprom_data.get_group_id();
      new_wifi = eeprom_data.get_wifi_name();
      new_pass = eeprom_data.get_wifi_password(); //There is an issue when parsing password - Current issue doesn't affect device 

      Serial.print("Group id: ");
      Serial.println(new_group);
      Serial.print("Wifi name: ");
      Serial.println(new_wifi);
      Serial.print("Password: ");
      Serial.println(new_pass);
    }
  }
  else if (data_size < 5)
  {
    //if the eeprom is empty we go and start hotspot
    //and capture values there
    //we then go ahead and save those value captured!
    //In .attach() function -> Hotspot values

    Serial.println("There is no data!");
    Serial.println("So I will proceed to save the data after you input in the ap!");

    ap.startAP();
    ap.checkAP();

    groupID = ap.get_groupId();
    password = ap.get_password();
    network = ap.get_wifiName();

    eeprom_data.attach(groupID,network,password); // Populates object with new values

    eeprom_data.save_data();
    data_size = eeprom_data.found_data(); // Gets the new data size 
    eeprom_data.load_data(data_size);

    Serial.println("This is the new data from the eeprom!");

    group = eeprom_data.get_group_id();
    wifi = eeprom_data.get_wifi_name();
    pass = eeprom_data.get_wifi_password(); //There is an issue when parsing password - Current issue doesn't affect device 

    new_group = group;

  }
  CountItRequest.attach("www.countit.com","/api/office/" + new_group + "/score", Config_JsonObjectToParseFor);
  
  Sequence.initializationSequence();
}

void loop(){
  delay(2000);

  Serial.println("Starting api request");
  float data = CountItRequest.sendGET();
  Serial.print("This is the score: ");
  Serial.println(data);

  int api_value = int(data + 0.5); // This helps to approximate the float value 
  Serial.print("This is the approx value: ");
  Serial.println(api_value);
  Serial.println();

  controlChannels(api_value);
  delay(10000);
}

void controlChannels(int score){

    TurnOn channels(score);
    int* pin_numbers = channels.getPin();

    isSame = channels.arrayIsSame(pin_numbers, temp_pins);

    temp_pins = channels.copyArray(pin_numbers);

    Serial.println();
    
    if (!isSame)
    {
      Sequence.resetSignAllOff();
      Sequence.changeInApiSequence();
    }

    PWM signal(pin_numbers);
    signal.turnOn();

    delay(5000);
    
}

//This functions are for testing data types of variables.

void types(String a) {Serial.println("It's a String");}
void types(int a) {Serial.println("It's a int");}
void types(char* a) {Serial.println("It's a char*");}
void types(float a) {Serial.println("It's a float");}