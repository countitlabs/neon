// File: main.cpp
// Description: This is the main file for the COUNT.IT NEON IOT project
// Arduino Libraries needed: ArduinoJson Version: 5.13.2,  WifiNina Version: 1.3.0
// Note: This code is specific for Arduino Uno Wifi Rev2
// Author: Keith Low

#include "SPI.h";

#include "api.h";
#include "network.h";
#include "light_control.h";
#include "pwm.h";
#include "control_sequence.h";

Network Wifi("ASUS","frenchfries");
Api CountItRequest("www.countit.com","/api/office/6f434034-040f-4ac8-b2c6-f19585485b2c/score","score7");
ControlSequence Sequence;

void setup(){
  Serial.begin(9600);
  Serial.println("Starting network functions");

  Wifi.networkConnect();
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