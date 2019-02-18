#include<EEPROM.h>
#include "data.h";

String group;
String wifi;
String pass;
int data_size;

void setup() {
  Serial.begin(9600);
  Serial.println("starting!");
  delay(5000);
  Data eeprom_data("","","");
  
//  eeprom_data.clean_eeprom();
  
  data_size = eeprom_data.found_data();
  Serial.println(data_size);
  
  if (data_size > 5) //The addition of all delimeters is = 4 so if the len of eeprom_data is more than 5 then we have stuff
  {
    Serial.println("There is data!");
    
    eeprom_data.load_data(data_size);
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
    // Once we get each different parameter 
    // we go ahead and start the hotspot
    // if user takes longer than 5 minutes 
    // we go and try to connect the value
  }
  else if (data_size < 5)
  {
    //if the eeprom is empty we go and start hotspot
    //and capture values there
    //we then go ahead and save those value captured!
    //In .attach() function -> Hotspot values

    eeprom_data.attach("123123","asus","frenfries");

    Serial.println("There is no data!");
    Serial.println("So I will proceed to save the data!");

    eeprom_data.save_data();
    data_size = eeprom_data.found_data();
    eeprom_data.load_data(data_size);

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
}

void loop() {
}


