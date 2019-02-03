// File: main.cpp
// Description: This is the main file for the COUNT.IT NEON IOT project
// Arduino Libraries needed: ArduinoJson Version: 5.13.2,  WifiNina Version: 1.3.0
// Note: This code is specific for Arduino Uno Wifi Rev2
// Author: Keith Low

//TODO: take the group id value from access_point.h and set it on the api.h constructor.

#include "SPI.h";

#include "api.h";
#include "light_control.h";
#include "pwm.h";
#include "control_sequence.h";
#include "access_point.h";

AccessPoint ap("countitNeon","letsrun1234");

Api CountItRequest("","","");

ControlSequence Sequence;

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

  controlChannels(data); //Here is where the logic happens
  delay(10000);
}

void controlChannels(float score){
    TurnOn channels(score);
    int* pin_numbers = channels.getPin();
    PWM signal(pin_numbers);
    signal.turnOn();

    delay(5000);
    Sequence.resetSignAllOff();
}


//This functions are for testing data types of variables.

void types(String a) {Serial.println("It's a String");}
void types(int a) {Serial.println("It's a int");}
void types(char* a) {Serial.println("It's a char*");}
void types(float a) {Serial.println("It's a float");}
