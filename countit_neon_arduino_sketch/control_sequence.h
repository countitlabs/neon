// File: control_sequence.h
// Description: This class controls all light sequences 
// Note: This code is specific for Arduino Uno Wifi Rev2
// Author: Keith Low

class ControlSequence{
  public:
    void initializationSequence(){
        //Turns off each word every 0.5 seconds and blinks circle 3 times every 1 second
        pinMode(2,OUTPUT);
        pinMode(3,OUTPUT);
        pinMode(4,OUTPUT);
        pinMode(5,OUTPUT);
        pinMode(6,OUTPUT);

        digitalWrite(2,HIGH);
        delay(500);
        digitalWrite(3,HIGH);
        delay(500);
        digitalWrite(4,HIGH);
        delay(500);
        digitalWrite(5,HIGH);
        delay(500);
        digitalWrite(6,HIGH);
        delay(500);

        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        delay(1000);

        digitalWrite(2,HIGH);
        digitalWrite(3,HIGH);
        digitalWrite(4,HIGH);
        digitalWrite(5,HIGH);
        digitalWrite(6,HIGH);
        delay(1000);

        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        delay(1000);

        digitalWrite(2,HIGH);
        digitalWrite(3,HIGH);
        digitalWrite(4,HIGH);
        digitalWrite(5,HIGH);
        digitalWrite(6,HIGH);
        delay(1000);

        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        delay(1000);

        resetSignAllOff();
        delay(2000);
    }
    void resetSignAllOff(){
        Serial.println();
        Serial.println("Reseting Sign!");
        Serial.println("Setting all to HIGH = OFF NEON");
        Serial.println();
        //The neon sign will be off until signals are sent to it.
        //If HIGH is sent the channel is off
        //If LOW is sent the channel is on
        digitalWrite(2,HIGH);
        digitalWrite(3,HIGH);
        digitalWrite(4,HIGH);
        digitalWrite(5,HIGH);
        digitalWrite(6,HIGH);
    }

    void changeInApiSequence(){
        Serial.println();
        Serial.println("Changing sequence!");
        Serial.println("If api is changing sequence should show");
        Serial.println();
        digitalWrite(6,LOW);
        delay(1000);
        digitalWrite(6,HIGH);
        delay(1000);
        digitalWrite(6,LOW);
        delay(1000);
        digitalWrite(6,HIGH);
        delay(1000);
        digitalWrite(6,LOW);
        delay(1000);
        digitalWrite(6,HIGH);
        delay(1000);
        resetSignAllOff();
    }
};
