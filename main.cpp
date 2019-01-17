// File: main.cpp
// Description: This is the main file for the COUNT.IT NEON IOT project
// Arduino Libraries needed: Arduinojson, wifinina
// Note: This code is specific for Arduino Uno Wifi Rev2
// Author: Keith Low

#include "SPI.h"

#include "api.h"
#include "network.h"
#include "light_control.h";
#include "pwm.h";

Network Wifi("<WIFI NAME GOES HERE>","<WIFI PASSWORD GOES HERE>");
Api Test("www.countit.com","/api/office/6f434034-040f-4ac8-b2c6-f19585485b2c/score","score7");

void setup(){
  Serial.begin(9600);
  Serial.println("Starting network functions");
  Wifi.networkConnect();
  controlChannels();
}

void loop(){
  delay(2000);
  Serial.println("Starting api test");
  String data = Test.sendGET();
  Serial.println("This is the score: " + data);
  
  delay(10000);
}

void controlChannels(){
    TurnOn channels(10.23);
    int* pin_numbers = channels.getPin();
    PWM signal(pin_numbers);
    signal.setOutput();
}