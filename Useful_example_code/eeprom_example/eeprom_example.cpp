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
        EEPROM.write(i,val[i] );
      }
    }

    void read_data() {
      int size = group_id.length();
      char val[size];
      
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


