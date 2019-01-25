#include <SPI.h>
#include <WiFiNINA.h>


WiFiServer server(80);


class AccessPoint{
    String HTTP_req;
    boolean readingNetwork = false;
    boolean readingPassword = false;
    String password = "";
    String network = "";

    bool needCredentials = true;
    bool needWiFi = false;
    bool readyToGo = false;


    int status = WL_IDLE_STATUS;
  public:


    void startAP() {
      char apssid[] = "Weimin";
      char appass[] = "weimin1234";

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
        WiFi.end();
        setup();

        // while (true);

      }

      delay(1000);
      server.begin();
      printAPStatus();  
    }

    void checkAP() {
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
              if (c==',') {
                readingPassword = false; 
                readyToGo = true;
              }
              else if (c!='!') {
                password += c;
              }
            }

            if (readyToGo)
            {
              if (c==','){
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
                
                client.print("<button type=\"button\" onclick=\"SendText()\">Enter</button>");
                client.println("</body>");
                client.println("<script>");
                client.println("var network = document.querySelector('#network');");
                client.println("var password = document.querySelector('#password');");
                
                client.println("function SendText() {");
                client.println("nocache=\"&nocache=\" + Math.random() * 1000000;");
                client.println("var request =new XMLHttpRequest();");
                client.println("netText = \"&txt=\" + \"?\" + network.value + \"!\" + password.value + \"*\" ,&end=end\";");
                client.println("request.open(\"GET\", \"ajax_inputs\" + netText + nocache, true);");
                client.println("request.send(null)");
                client.println("password.value=''");
                client.println("network.value=''}");
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
        // Serial.println(network);
        // Serial.println(password);
        Serial.println();

        char network_val[15];
        char password_val[15];

        network.toCharArray(network_val,15);
        password.toCharArray(password_val,15);

        char *results[2] = {network_val,password_val};
        Serial.println();
        Serial.println("Here is the test!");
        Serial.println(results[0]);
        Serial.println(results[1]);

        //To do:

        //send network and password variables to the network.h class and test it through the objects
        //strToChar function doesnt work, so we have to find a function to convert this values
        //Either we return an array with the three values network, password and group id
        //or we just test the same code with the proper strtochar function
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

        char network_val[15];
        char password_val[15];

        network.toCharArray(network_val,15);
        password.toCharArray(password_val,15);

        WiFi.begin(network_val, password_val);
        delay(10000);
        }
      Serial.println("WiFi connection successful");
      printWiFiStatus();
      needWiFi = false;
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