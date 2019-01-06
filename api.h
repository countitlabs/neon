// File: api.h 
// Description: Allows a get request to any api.
// Last updated: 01/06/19

#include "ArduinoJson.h"
#include "WiFi101.h"

class Api {
  const char * server;
  String query;
  String json_data;

  public:
    WiFiClient client;
    Api(const char * url_endpoint, String query_new, String json)
    {
      server = url_endpoint;
      query = query_new;
      json_data = json;
    }
    void sendGET() //client function to send/receive GET request data.
    {
      Serial.print("Retrying connection!");
      if (client.connect(server, 80)) {  //starts client connection, checks for connection
        Serial.println("connected");
        client.println("GET " + query + " HTTP/1.0"); // https://hackingmajenkoblog.wordpress.com/2016/02/04/the-evils-of-arduino-strings/
        Serial.println("GET " + query + " HTTP/1.0");
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
        JsonObject& root = jsonBuffer.parseObject(line);
        String text = root[json_data];
        Serial.println("This is the parsed data: " + text);

        // if (text.length() != 0)
        // {
        //   Serial.println("Found something!, returning now!");
        //   client.stop();
        //   return;
        // }
        // ISSUE: cannot make constant requests because reponse returns empty after the first request

        // Serial.println(text.length());
        // if (text.length() == 0){
        //   return;
        // }
        // if (!root.success()) {
        //   Serial.println("parseObject() failed");
        //   return;
        // }
        // String text = root[json_data];
        // Serial.println(text);
      }
    }
};