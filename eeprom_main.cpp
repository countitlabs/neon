#include<EEPROM.h>
#include "data.h";

String group;
String wifi;
String pass;

void setup() {
  Serial.begin(9600);
  Serial.println("starting!");
  delay(5000);
  Data eeprom_data("","","");
  
  if (eeprom_data.found_data() > 5) //The addition of all delimeters is = 4 so if the len of eeprom_data is more than 5 then we have stuff
  {
    Serial.println("There is data!");
    int len = eeprom_data.found_data();
    eeprom_data.load_data(len);
    delay(2000);

    group = eeprom_data.get_group_id();
    wifi = eeprom_data.get_wifi_name();
    pass = eeprom_data.get_wifi_password();

    Serial.print("group id: ");
    Serial.println(group);
    Serial.print("Wifi name: ");
    Serial.println(wifi);
    Serial.print("Password: ");
    Serial.println(pass);
  }
  else if (eeprom_data.found_data() < 5)
  {
    //value shoud come in here
    eeprom_data.attach()
    Serial.println("There is no data!");
    Serial.println("So I will proceed to save the data!");
    eeprom_data.save_data();
  }

  eeprom_data.attach(group, wifi, pass);
  
}

void loop() {
}

// We get the data from the ap - group id, wifi and password
// * These three values are passed into the eeprom object to be saved
// the ap connects and then we save the data into the eeprom


// if user restarts 
// eeprom is checked to find data - if found data = true. We then run normal code
// and have a counter for 10 minutes - then proceed to run code normally 
// if user doesn't input code for 10 minutes the code should proceed to run 
// based on the data in the eeprom

