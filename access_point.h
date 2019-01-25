#include <SPI.h>
#include <WiFiNINA.h>

char apssid[] = "CountitNeon";
char appass[] = "countit1234";
int status = WL_IDLE_STATUS;
WiFiServer server(80);
String HTTP_req;
boolean readingNetwork = false;
boolean readingPassword = false;
boolean readingGroupId = false;

String password = "";
String network = "";
String groupId = "";


boolean needCredentials = true;
boolean needWiFi = false;
boolean connectPubNub = false;
boolean readyToGo = false;

char *strToChar(String str) {
  int len = str.length() + 1;
  char c[len];
  str.toCharArray(c, len);
  return c;
}

void setup() {
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
    Serial.println("Calling setup function again!");
    Serial.println();
    setup();

    // while (true);

  }

  delay(1000);
  server.begin();
  printAPStatus();  
}

void loop() {
  if (needCredentials) {
    getCredentials();
  }
  if (needWiFi) {
    getWiFi();
  }
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
            client.println("<style type=\"text/css\"> body {font-family: sans-serif; margin:50px; padding:20px; line-height: 250% } </style>");
            client.println("<title>Arduino Setup</title>");
            client.println("</head>");
            client.println("<body>");

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
            client.println("groupId.value=''}");
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

    Serial.println(network);
    Serial.println(password);
    Serial.println(groupId);
  }
}

void getWiFi () {
  if (network == "" or password == "") {
        Serial.println("Invalid WiFi credentials");
        while (true);
      }
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");

    char network_val[40];
    char password_val[40];

    network.toCharArray(network_val,40);
    password.toCharArray(password_val,40);

    Serial.println(network_val);

    WiFi.begin(network_val, password_val);
    delay(10000);
    }
  Serial.println("WiFi connection successful");
  printWiFiStatus();
  needWiFi = false;
  connectPubNub = true;
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