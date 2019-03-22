// File: access_point.h
// Description: This class allows the creation of an access point and an interface to connect
// to the wifi selected by the user.
// Note: This code is specific for Arduino Uno Wifi Rev2
// Author: Keith Low

#include <SPI.h>
#include <WiFiNINA.h>
#include <elapsedMillis.h> // Timer for eeprom

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
  int interval;

  char groupId_val[40];
  char network_val[40];
  char password_val[40];

  public:
    AccessPoint(const char * wifi_name, const char * password, int timer_range){
      apssid = wifi_name;
      appass = password;
      interval = timer_range;
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

    //  No timer here
    void checkAP() {
      while (!doneChecking){
        if (needCredentials) {
          getCredentials();
        }
        if (needWiFi) {
          getWiFi();
        }
      }
    }

    //  Timer for eeprom
    bool checkAPWithTimer() {
      elapsedMillis timeElapsed;
      bool timerIsDone = false;

      while (!doneChecking && !timerIsDone){
        Serial.print("Ellapsed time: ");
        Serial.println(timeElapsed);

        if (timeElapsed > interval) {
          Serial.println("It is working!");
          timerIsDone = true;
        } 

        if (needCredentials) {
          getCredentials();
        }
        if (needWiFi) {
          getWiFi();
        }
      }
      if (timerIsDone) {
        Serial.println("Timer is done, so we go and run direct connection");
        return true;
      } 
      
      Serial.println("User submitted something!");
      return false;
    }

    String get_groupId(){
      return groupId;
    }

    String get_password(){
      return password;
    }

    String get_wifiName(){
      return network;
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
                client.println("<style type=\"text/css\"> body {background-color: #00adef;font-family: sans-serif; margin:50px; padding:20px; line-height: 250% } </style>");
                client.println("<title>Count.It Neon Setup</title>");
                client.println("</head>");
                client.println("<body>");

                client.println("<h1 style=\"font-family: \"Lucida Sans Unicode\", \"Lucida Grande\", sans-serif; font-size:8vw; color:white; margin-left:100px;\">COUNT.IT NEON</h1>");

                client.println("<h3>Enter WiFi Credentials</h3>");
                client.print("<h4>NETWORK NAME: </h4>");
                client.print("<input id=\"network\"/><br>");
                client.print("<h4>PASSWORD: </h4>");
                client.print("<input id=\"password\"/><br>");

                client.println("<h3>Enter Count.It Group ID</h3>");
                client.print("<h4>GROUP ID: </h4>");
                client.print("<input id=\"groupId\"/><br>");
                client.print("<br>");
                
                client.print("<button style=background-color:#ffffff;border: 1px solid black;color: black;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer  type=\"button\" onclick=\"SendText()\">Submit</button>");

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
                client.println("sendAlert();}");

                client.println("function sendAlert() {");
                client.println("alert(\"Success! You have submitted your WiFi network credentials and Count.It group ID. Give your sign a moment to connect!\");");
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

    //  Function created to directly connect to wifi with eeprom data
    void connectDirectlyToWifi(const String& wifi, const String& wifi_password){
      char wifi_array[40];
      char wifi_password_array[40];

      if (wifi == "" or wifi_password == "") {
            Serial.println("Invalid WiFi credentials");
            while (true);
      }
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");

        Serial.println(wifi);

        wifi.toCharArray(wifi_array,40);
        wifi_password.toCharArray(wifi_password_array,40);

        WiFi.begin(wifi_array, wifi_password_array);
        delay(10000);
      }
      Serial.println("WiFi connection successful");
      printWiFiStatus();
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
