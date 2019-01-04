// File: wifi_connection.cpp
// Description: This file allows mkr1000 board connection to a wpa wifi source and makes a get request to the worldclock api.
// Last updated: 01/03/19
// Libraries added:Arduinojson, WiFi101

#include "SPI.h"
#include "api.h"
#include "network.h"

Network Wifi("<WIFI SSID GOES HERE>","<WIFI PASSWORD>");
Api Test("worldclockapi.com","/api/json/est/now","currentDateTime");

void setup(){
  Serial.begin(9600);
  Serial.println("Starting network functions");
  Wifi.networkConnect();
}

void loop(){
  delay(20000);
  Serial.println("Starting api test");
  Test.sendGET();
}