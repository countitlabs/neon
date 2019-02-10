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
  bool currentLineIsBlank = false;
  unsigned long byteCount = 0;

  public:
    WiFiSSLClient client;
    Api(const char * url_endpoint, String query_new, String json)
    {
      
    }
    void attach(const char * url_endpoint, String query_new, String json)
    {
      server = url_endpoint;
      query = query_new;
      json_data = json;
    }

    float sendGET() //client function to send/receive GET request data.
    {
        if (client)
        {
          bool currentLineIsBlank = true;
        }
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
        delay(2000);
        while (client.connected())
        {
          if (client.available()){
            char c = client.read();
            if (c == '\n' && currentLineIsBlank)
            {
              // This is after the header. The POST data is in the body.
              // so read the POST data
              Serial.println("Reading!");
              while(client.available())
              {
                // Serial.write(client.read());
                String data = "";
                data = client.readStringUntil('\n');
                float text = parseData(data,json_data);
                Serial.println(text);
                return text;

              }
            }
            else if (c == '\n')
            {
              currentLineIsBlank = true;
            }
            else if (c != '\r')
            {
              currentLineIsBlank = false;
            }
          }

        }
      Serial.println("Done");
    }
    float parseData(String data, String json_data)
    {
      const size_t capacity = JSON_OBJECT_SIZE(7) + 180;
      DynamicJsonBuffer jsonBuffer(capacity);
      JsonObject& root = jsonBuffer.parseObject(data);
      float score = root[json_data];
      return score;
    }
};