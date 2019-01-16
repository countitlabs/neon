// File: api.h 
// Description: This class allows a Arduino Uno Wifi Rev2 to send a HTTPS GET request to any api.
// The api host has to be entered as the first argument of the class
// and the query as the second argument, and lastly the json object you are looking to get at the end.
// Author: Keith Low

//Things to work on:
//Clean the get request on line 56 - I am getting a lot of garbage make sure to be able to parse it

#include "ArduinoJson.h"
#include <WiFiNINA.h>

class Api {
  const char * server;
  String query;
  String json_data;
  bool printWebData = true;
  unsigned long byteCount = 0;

  public:
    WiFiSSLClient client;
    Api(const char * url_endpoint, String query_new, String json)
    {
      server = url_endpoint;
      query = query_new;
      json_data = json;
    }
    void sendGET() //client function to send/receive GET request data.
    {
      Serial.print("Starting connection!");
        if (client.connect(server, 443)) {  //starts client connection, checks for connection
          Serial.println("connected");
          client.println("GET " + query + " HTTP/1.1"); // https://hackingmajenkoblog.wordpress.com/2016/02/04/the-evils-of-arduino-strings/
          Serial.println("GET " + query + " HTTP/1.1");
          client.println("Host: " + String(server)); // This needs to be changed to save memory
          client.println("Connection: close"); 
          client.println(); //end of get request
        }
        else {
          Serial.println("connection failed"); //error message if no client connect
          Serial.println();
          return;
        }
      delay(10000);
      String line = "";
      StaticJsonBuffer<5000> jsonBuffer;
      while (client.connected()) {
        line = client.readStringUntil('\n');
        Serial.println("****** This is the response: " + line);
        int len = client.available();
        if (len > 0) {
          byte buffer[80];
          if (len > 80) len = 80;
          client.read(buffer, len);
          if (printWebData) {
            Serial.println("Here is writing");
            Serial.write(buffer, len); // show in the serial monitor (slows some boards)
            Serial.println("Here is writing");
          }
          byteCount = byteCount + len;
        }
        // JsonObject& root = jsonBuffer.parseObject(line);
        // String text = root[json_data];
        // Serial.println("######## This is the parsed data: " + text);

      }
    }
};