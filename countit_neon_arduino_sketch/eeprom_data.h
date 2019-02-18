//Save group id:

// Start with #<group_id>! end with exclamation mark

//To save wifi:

// Read until there is an ! and write <wifi_name>@ end with @ mark

//To save password:

// Read until there is an @ and write <Password>* end with * mark

// ie : #d4313b24-ab5f-42ef-aba9-527b84757000!asus@frenchfries*

//To read:

//Group id read for # and end with !
//wifi_name read for ! and end with @
//password read for @ and end with *

    // The user will input once the data will be saved!
    // ** Function that checks the device and if there is anything written into the eeprom return true
    // If the user restarts the device, the device should check if there is any data in eeprom
    // If there is data wait 10 minutes, and if nothing happens - Proceed with that data.

#include <EEPROM.h>

class Data{
  
  String group_id;
  String wifi_name;
  String wifi_pass;

  String group_id_read;
  String wifi_read;
  String password_read;
  String all_data;

  int size;

  char f;

  bool completely_done = false;
  bool reading_wifi_name = false;
  bool reading_password = false;
  bool reading_group_id;

  public:
    Data(String group_number, String wifi, String pass)
    {

    }

    void attach(String group_number, String wifi, String pass)
    { 
       group_id = "#" + group_number + "!";
       wifi_name = wifi + "@";
       wifi_pass = pass + "*";
    }

    void initialize_global_variables() {
      size = group_id.length() + wifi_name.length() + wifi_pass.length();
    }

    void save_data() {

      initialize_global_variables();

      if (group_id.length() > 3 && wifi_name.length() > 2){
        all_data = group_id + wifi_name + wifi_pass;
      } 
      else 
      {
        Serial.println("GROUP ID OR WIFI NAME ARE empty");
        while(true); // If group id or wifi are entered empty the device will not do anything until restart!
      }
      Serial.println(all_data);

      char val[size];
        
      Serial.println("Saving");
      all_data.toCharArray(val,size + 1);
      Serial.println(sizeof(val));


      for(int i=0 ; i < sizeof(val); i++)
      {
        if (EEPROM.read(i) != val[i])
        {
          delay(100);
          Serial.println("That character does not exists");
          EEPROM.write(i,val[i]);
        } 
        else 
        {
          delay(100);
          Serial.println("That character exists");
          Serial.println(val[i]);
        }
      }
    }

    int found_data() {

      bool thereIsData = false;

      String data;

      byte b = 0;

      while (!thereIsData)
      {
        b += 1;
        f = EEPROM.read(b);
        data += (f);
        Serial.println(data);
        if (f == '*')
        {
          if (data.length() > 5) // Make sure the data is formatted in that way
          {
            thereIsData = true;
            Serial.print("This is the data: ");
            Serial.println(data);
          }
        }
        else if (data == 000) //check if the devices is empty
        {
          Serial.println("Its empty");
          return 0;
        }
      }
      Serial.print("This is the size: ");
      Serial.println(data.length());
      return data.length();
    }

    void load_data(int size) {
      char val[size];
      
      Serial.println("reading!");
      
      group_id.toCharArray(val,size + 1);

      for(byte b=0 ; b < sizeof(val) && completely_done == false; b++)
      {
        f = EEPROM.read(b);
        if (f =='#')
        {
          reading_group_id = true;
        }

        if (reading_group_id)
        {
          Serial.println("Reading group id");
          if (f =='!')
          {
            reading_wifi_name = true;
            reading_group_id = false;
            Serial.println("Group id is now false");
          }
          else if (f !='#')
          {
            group_id_read += (f);
          }
        }

        if (reading_wifi_name)
        {
          Serial.println("Reading wifi name");
          if (f =='@')
          {
            reading_password = true;
            reading_wifi_name = false;
          }
          else if (f !='!')
          {
            wifi_read += (f);
          }
        }

        if (reading_password)
        {
          Serial.println("Reading password");
          if (f == '*')
          {
            completely_done = true;
            reading_password = false;
          }
          else if (f !='@')
          {
            password_read += (f);
          }
        }
      }
    }

    void clean_eeprom()
    {
      Serial.println("Clearing eeprom everything to zero!");
      for (int i = 0 ; i < 255 ; i++) {
        EEPROM.write(i, 0);
        Serial.println(i);
      }
    }

    String get_group_id()
    {
      return group_id_read;
    }

    String get_wifi_name()
    {
      return wifi_read;
    }

    String get_wifi_password()
    {
      return password_read;
    }
};
