//File: pwm.h
//Description: This class allows the user to send a pwm signal to any specific pins written in an array
//And passing it as an argument to the class
//Author: Keith Low

//LOW should turn on the sign

class PWM {
  int pins[5]; //needs to be changed to save mem
  public:
    PWM(int* pin_out){
      for (int i = 0; i < 5; i++){
        pins[i] = pin_out[i];
      }
    }
    void turnOn(){
      for (int i = 0; i < 5; i++){
        if (pins[i] > 1 && pins[i] < 7){
          Serial.print("SENDING LOW FOR: ");
          Serial.println(pins[i]);
          delay(1000);
          digitalWrite(pins[i], LOW);
        }
      }
    }
};

