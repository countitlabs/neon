//Save group id:

// Start with #<group_id>! end with exclamation mark

//To save wifi:

// Read until there is an ! and write <wifi_name>@ end with @ mark

//To save password:

// Read until there is an @ and write <Password>* end with * mark

//To read:

//Group id read for # and end with !
//wifi_name read for ! and end with @
//password read for @ and end with *

#include <EEPROM.h>

class Data{
  
  String group_id;
  String IPis;

  public:
    Data(String group_number)
    {
      group_id = group_number;
    }

    void save_data() {

      int size = group_id.length();
      char val[size];
        
      Serial.println("Saving");
      group_id.toCharArray(val,size + 1);
      Serial.println(sizeof(val));


      for(int i=0 ; i < sizeof(val); i++)
      {
        if (EEPROM.read(i) != val[i])
        {
          Serial.println("That character does not exists");
          EEPROM.write(i,val[i]);

        } 
        else 
        {
          Serial.println("That character exists");
          Serial.println(val[i]);
        }
      }
    }

    void read_data() {
      int size = group_id.length();
      char val[size];

      Serial.println(size);
      
      Serial.println("reading!");
      
      group_id.toCharArray(val,size + 1);

      for(byte b=0 ; b < sizeof(val); b++)
      {
        char f = EEPROM.read(b);
        IPis += (f);
      }
      Serial.println(IPis);
    }
};