// File: main.cpp
// Description: This is the main file for the COUNT.IT NEON IOT project
// Arduino Libraries needed: ArduinoJson Version: 5.13.2,  WifiNina Version: 1.3.0 and elapsedMilis v 1.0.4
// Note: This code is specific for Arduino Uno Wifi Rev2
// Author: Keith Low

//TODO: Store group id value and wifi into eeprom for 10 minutes if user decides not to re enter, just reuse those values

#include "SPI.h";

#include "api.h";
#include "light_control.h";
#include "pwm.h";
#include "control_sequence.h";
#include "access_point.h";

#include "eeprom_data.h";

#include <elapsedMillis.h> // This library is currently not used 


// Objects
//********************************

AccessPoint ap("CountItNeon","letsrun1234");

Api CountItRequest("","","");

ControlSequence Sequence;

Data eeprom_data("","","");

//********************************

//Global Variables:
//********************************
//Light Control
int* temp_pins;
bool isSame = true;
//********************************

//********************************
//AccesPoint
String groupID;
String network;
String password;
//********************************

//********************************
//eeprom data
int data_size;
String group;
String wifi;
String pass;
//********************************

void setup(){
  Serial.begin(9600);
  Serial.println("Starting network functions");

  data_size = eeprom_data.found_data();
  Serial.println(data_size);

  //The addition of all delimeters is = 4 
  //so if the len of eeprom_data is more than 5 then we have stuff

  if (data_size > 5) 
  {
    elapsedMillis timeElapsed; // Starts timer

    Serial.println("There is data!");
    
    eeprom_data.load_data(data_size);
    delay(2000);

    group = eeprom_data.get_group_id();
    wifi = eeprom_data.get_wifi_name();
    pass = eeprom_data.get_wifi_password();

    Serial.print("group id: ");
    Serial.println(group);
    Serial.print("Wifi name: ");
    Serial.println(wifi);
    Serial.print("Password: ");
    Serial.println(pass);
    // Once we get each different parameter 
    // we go ahead and start the hotspot
    // if user takes longer than 5 minutes 
    // we go and try to connect the value

    //try to connect to wifi via wifi and pass variables

    Serial.println("Attempting direct wifi!");
    ap.connectDirectlyToWifi(wifi,pass);

    // Need to start ap and wait 5 minutes 
    // if 5 minutes passed then go ahead and directly connect to ap.

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

    group = eeprom_data.get_group_id();
    wifi = eeprom_data.get_wifi_name();
    pass = eeprom_data.get_wifi_password();
  }
  CountItRequest.attach("www.countit.com","/api/office/" + group + "/score","score7");
  
  Sequence.initializationSequence();
}

void loop(){
  delay(2000);

  Serial.println("Starting api test");
  float data = CountItRequest.sendGET();
  Serial.print("This is the score: ");
  Serial.println(data);

  int api_value = int(data + 0.5); // This helps to approximate the float value 
  Serial.print("This is the approx value");
  Serial.println(api_value);

  controlChannels(api_value); //Here is where the logic happens
  delay(10000);
}

void controlChannels(int score){

    TurnOn channels(score);
    int* pin_numbers = channels.getPin();

    Serial.println("This is the array");
    Serial.println(pin_numbers[0]);
    Serial.println(pin_numbers[1]);
    Serial.println(pin_numbers[2]);
    Serial.println(pin_numbers[3]);

    isSame = channels.arrayIsSame(pin_numbers, temp_pins);

    temp_pins = channels.copyArray(pin_numbers);

    Serial.println("This is the temp array");
    Serial.println(temp_pins[0]);
    Serial.println(temp_pins[1]);
    Serial.println(temp_pins[2]);
    Serial.println(temp_pins[3]);

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
