//File: pwm.h
//Description: This class allows the user to send a pwm signal to any specific pins written in an array
//And passing it as an argument to the class
//Author: Keith Low

class PWM {
  int* pins = new int[5];
  public:
    PWM(int* pin_out){
      for (int i = 0; i < 5; i++){
        pins[i] = pin_out[i];
      }
    }
    void setOutput(){
      for (int i = 0; i < 5; i++){
        if (pins[i] >= 1 && pins[i] <= 7){
          Serial.print("SETTING OUTPUT FOR: ");
          Serial.println(pins[i]);
          delay(1000);
          pinMode(pins[i],OUTPUT);
        }
          
      }
      turnOn();
    }
    void turnOn(){
      for (int i = 0; i < 5; i++){
        if (pins[i] >= 1 && pins[i] <= 7){
          Serial.print("SENDING HIGH FOR: ");
          Serial.println(pins[i]);
          delay(1000);
          digitalWrite(pins[i], HIGH);
        }
      }
    }
};

