// File: wifi_connection.cpp
// Description: This file allows mkr1000 board connection to a wpa wifi source and makes a get request to the worldclock api
// Last updated: 01/02/19

// Libraries added:
// Arduinojson library
// Wifi101 library

#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoJson.h>

class Network {
  String ssid;
  String pass;
  int wifiStatus = WL_IDLE_STATUS;

  public:
    Network(String wifi_name, String password){
      ssid = wifi_name;
      pass = password;
    }

    bool networkConnect() {
       // check for the presence of the shield:
      if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        // don't continue:
        while (true);
      }

      // attempt to connect to WiFi network:
      Serial.print("SSID: ");
      Serial.println(ssid);
      // Attempt to connect 10 times
      int attempts = 1;
      while (wifiStatus != WL_CONNECTED && attempts < 6) {
        Serial.println("Attempt " + String(attempts));
        // Connect to WPA/WPA2 network. WEP Network requires additional parameter.
        wifiStatus = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
        attempts++;

      }

      // Print WiFi status:
      if ( wifiStatus != WL_CONNECTED ) {
        Serial.println("Cannot Connect to Network.");
        return false;
      } else {
       Serial.println("Connected.");
       printCurrentNet();
       printWifiData();
       return true;
      }
    }

    void printCurrentNet() {
      // print the SSID of the network you're attached to:
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID());

      // print the MAC address of the router you're attached to:
      byte bssid[6];
      WiFi.BSSID(bssid);
      Serial.print("BSSID: ");
      printMacAddress(bssid);

      // print the received signal strength:
      long rssi = WiFi.RSSI();
      Serial.print("signal strength (RSSI):");
      Serial.println(rssi);

      // print the encryption type:
      byte encryption = WiFi.encryptionType();
      Serial.print("Encryption Type:");
      Serial.println(encryption, HEX);
      Serial.println();
    }

    void printWifiData() {
      // print your WiFi shield's IP address:
      IPAddress ip = WiFi.localIP();
      Serial.print("IP Address: ");
      Serial.println(ip);
      Serial.println(ip);

      // print your MAC address:
      byte mac[6];
      WiFi.macAddress(mac);
      Serial.print("MAC address: ");
      printMacAddress(mac);
    }

    void printMacAddress(byte mac[]) {
      for (int i = 5; i >= 0; i--) {
        if (mac[i] < 16) {
          Serial.print("0");
        }
        Serial.print(mac[i], HEX);
        if (i > 0) {
          Serial.print(":");
        }
      }
        Serial.println();
    }
};


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

        if (text.length() != 0)
        {
          Serial.println("Found something!, returning now!");
          client.stop();
          return;
        }
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

Network Wifi("<WIFI SSID GOES HERE>","<WIFI PASSWORD>");
Api Test("worldclockapi.com","/api/json/est/now","currentDateTime");

void setup(){
  Serial.println("Starting network functions");
  Wifi.networkConnect();
}

void loop(){
  delay(20000);
  Serial.println("Starting api test");
  Test.sendGET();
}