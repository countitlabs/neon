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

#include <elapsedMillis.h> // This library is currently not used 


AccessPoint ap("CountItNeon","letsrun1234");

Api CountItRequest("","",""); //Using empty constructor to access the object globally on setup() and loop()

ControlSequence Sequence;

int* temp_pins;
bool isSame = true;

void setup(){
  Serial.begin(9600);

  Serial.println("Starting network functions");

  ap.startAP(); // Starting accespoint 
  char * groupId_val;
  groupId_val = ap.checkAP(); // Group id values

  CountItRequest.attach("www.countit.com","/api/office/" + String(groupId_val) + "/score","score7");

  //At this point the device should have access to wifi
  
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
//    temp_pins[3] = 12;

    Serial.println("This is the temp array");
    Serial.println(temp_pins[0]);
    Serial.println(temp_pins[1]);
    Serial.println(temp_pins[2]);
    Serial.println(temp_pins[3]);

    Serial.println();
    
    
    if (!isSame)
    {
      //If the arrays are not the same we blink
      Sequence.resetSignAllOff();
      Sequence.changeInApiSequence();
    }
    // otherwise we don't blink at all

    PWM signal(pin_numbers);
    signal.turnOn();

    delay(5000);
    
}

// void testing_function(){
//   if (test.found_data() > 5) //The addition of all delimeters is = 4 so if the len of test is more than 5 then we have stuff
//   {
//     Data eeprom_data("","","");
//     Serial.println("There is data!");
//     int len = test.found_data();
//     test.load_data(len);
//     delay(2000);
//     String group = test.get_group_id();
//     String wifi = test.get_wifi_name();
//     String pass = test.get_wifi_password();
//     Serial.print("group id: ");
//     Serial.println(group);
//     Serial.print("Wifi name: ");
//     Serial.println(wifi);
//     Serial.print("Password: ");
//     Serial.println(pass);

//     elapsedMillis timeElapsed; // This starts the counter

//     // if timeElapsed > 300000 then input the data in to the 

//   }
//   else if (test.found_data() < 5)
//   {
//     Serial.println("There is no data!");
//     Serial.println("So I will proceed to save the data!");
//     test.save_data();
//   }
// }


//This functions are for testing data types of variables.

void types(String a) {Serial.println("It's a String");}
void types(int a) {Serial.println("It's a int");}
void types(char* a) {Serial.println("It's a char*");}
void types(float a) {Serial.println("It's a float");}