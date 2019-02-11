// File: access_point.h
// Description: This class allows the creation of an access point and an interface to connect
// to the wifi selected by the user.
// Note: This code is specific for Arduino Uno Wifi Rev2
// Author: Keith Low

#include <SPI.h>
#include <WiFiNINA.h>

WiFiServer server(80);

class AccessPoint {
  int status = WL_IDLE_STATUS;
  bool readingNetwork = false;
  bool readingPassword = false;
  bool readingGroupId = false;
  bool doneChecking = false;

  String password = "";
  String network = "";
  String groupId = "";

  bool needCredentials = true;
  bool needWiFi = false;
  bool readyToGo = false;

  const char * apssid;
  const char * appass;

  char groupId_val[40];
  char network_val[40];
  char password_val[40];

  public:
    AccessPoint(const char * wifi_name, const char * password){
      apssid = wifi_name;
      appass = password;
    }
    void startAP() {
      WiFi.end();
      Serial.begin(9600);
      Serial.println("Access Point Web Server");
      
      if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        while (true);
      }
      Serial.print("Creating access point named: ");
      Serial.println(apssid);

      status = WiFi.beginAP(apssid,appass);
      delay(5000);
      
      if (status != WL_AP_LISTENING) {
        Serial.println("Creating access point failed");
        Serial.println();
        delay(1000);
        Serial.println("Calling startAP function again! Restarting process");
        Serial.println();
        startAP();
      }
      delay(1000);
      server.begin();
      printAPStatus();  
    }

    char * checkAP() {
      while (!doneChecking){
        if (needCredentials) {
          getCredentials();
        }
        if (needWiFi) {
          getWiFi();
        }
      }
      Serial.print("This is the group id: ");
      Serial.println(groupId);
      groupId.toCharArray(groupId_val,40);
      return groupId_val;
    }

    void getCredentials() {
      WiFiClient client = server.available();
      if (client) {                            
        Serial.println("new client");           
        String currentLine = "";               
        while (client.connected()) {            
          if (client.available()) {            
            char c = client.read();           
            Serial.print(c);
            if (c=='?') readingNetwork = true;
            if (readingNetwork) {
              if (c=='!') {
                readingPassword = true;
                readingNetwork = false;
              }
              else if (c!='?') {
                network += c;
              }
            }
            if (readingPassword) {
              if (c=='*') {
                readingGroupId = true;
                readingPassword = false; 
              }
              else if (c!='!') {
                password += c;
              }
            }

            if (readingGroupId) {
              if (c==',') {
                readyToGo = true;
                readingGroupId = false;
              }
              else if (c!='*') {
                groupId += c;
              }
            }

            if (readyToGo) {
              Serial.println();
              Serial.print("Network Name: ");
              Serial.println(network);
              Serial.print("Password: ");
              Serial.println(password);
              Serial.println();
              client.stop();
              WiFi.end();
              needCredentials = false;
              needWiFi = true; 
            }
            if (c == '\n') {   
              if (currentLine.length() == 0) {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();
                client.println("<html>");
                client.println("<head>");
                client.println("<style type=\"text/css\"> body {background-color: #3E92CC ;font-family: sans-serif; margin:50px; padding:20px; line-height: 250% } </style>");
                client.println("<title>Arduino Setup</title>");
                client.println("</head>");
                client.println("<body>");

                client.println("<h1 style=\"font-family: \"Lucida Sans Unicode\", \"Lucida Grande\", sans-serif; font-size:8vw; color:white; margin-left:100px;\">Count.It Neon Light</h1>");

                client.println("<h2>WIFI CREDENTIALS</h2>");
                client.print("NETWORK NAME: ");
                client.print("<input id=\"network\"/><br>");
                client.print("PASSWORD: ");
                client.print("<input id=\"password\"/><br>");

                client.println("<h2>Countit</h2>");
                client.print("Group ID: ");
                client.print("<input id=\"groupId\"/><br>");
                client.print("<br>");
                
                client.print("<button type=\"button\" onclick=\"SendText()\">Enter</button>");

                client.println("</body>");
                client.println("<script>");
                client.println("var network = document.querySelector('#network');");
                client.println("var password = document.querySelector('#password');");
                client.println("var groupId = document.querySelector('#groupId');");
                
                client.println("function SendText() {");
                client.println("nocache=\"&nocache=\" + Math.random() * 1000000;");
                client.println("var request =new XMLHttpRequest();");
                client.println("netText = \"&txt=\" + \"?\" + network.value + \"!\" + password.value + \"*\" + groupId.value + \",&end=end\";");
                client.println("request.open(\"GET\", \"ajax_inputs\" + netText + nocache, true);");
                client.println("request.send(null)");
                client.println("network.value=''");
                client.println("password.value=''");
                client.println("groupId.value=''");
                client.println("sendTest();}");

                client.println("function sendTest() {");
                client.println("alert(\"You have succesfully submitted wifi details! Give the device a couple of seconds to connect!\");");
                client.println("}");

                client.println("</script>");
                client.println("</html>");
                client.println();
                break;
              }
              else { 
                currentLine = "";
              }
            }
            else if (c != '\r') { 
              currentLine += c;
            }
          }
        }
        client.stop();
        Serial.println("client disconnected");
        Serial.println();

        if (network != "") {
          Serial.print("The network: ");
          Serial.println(network);
          Serial.print("The password: ");
          Serial.println(password);
          Serial.print("The groupId: ");
          Serial.println(groupId);
          Serial.println();
        } else {
          Serial.println("User has not submitted any inputs!");
        }
      }
    }

    void getWiFi () {
      if (network == "" or password == "") {
            Serial.println("Invalid WiFi credentials");
            while (true);
          }
        while (WiFi.status() != WL_CONNECTED) {
          Serial.print("Attempting to connect to SSID: ");

          Serial.println(network);

          network.toCharArray(network_val,40);
          password.toCharArray(password_val,40);

          WiFi.begin(network_val, password_val);
          delay(10000);
        }
      Serial.println("WiFi connection successful");
      printWiFiStatus();
      needWiFi = false;
      doneChecking = true;
      delay(1000);
    }

    void printWiFiStatus() {
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID());
      IPAddress ip = WiFi.localIP();
      Serial.print("IP Address: ");
      Serial.println(ip);
      Serial.print("signal strength (RSSI):");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
    }

    void printAPStatus() {
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID());
      IPAddress ip = WiFi.localIP();
      Serial.print("IP Address: ");
      Serial.println(ip);
      Serial.print("signal strength (RSSI):");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
      Serial.print("To connect, open a browser to http://");
      Serial.println(ip);
    }
};