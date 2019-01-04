// File: network.h 
// Description: This class allows the connection to a WPA wifi source.
// Last updated: 01/03/19

#include "WiFi101.h"

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