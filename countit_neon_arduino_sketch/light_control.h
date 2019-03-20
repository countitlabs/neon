//File: light_control.h
//Description: This class allows the user to automate the set up of channels to be turned on or off.
//Author: Keith Low

// PSEUDO CODE REPRESENTATION:

// 5 CHANNELS
// Super !!! = Channel 2 or pin 2
// In = Channel 3 or pin 3
// Active = Channel 4 or pin 4
// Ish =  Channel 5 or pin 5
// Circle = Channel 6 or pin 6

// 1- If score >= 1 && score <= 10 turn on [Super] [In] [Active] [!!] ---> Channels 2,3,4
// 2- If score >= 11 && score <= 20 turn on [Super] [In] [Active] [Ish] [!!] ---> Channels 2,3,4,5
// 3- If score >= 21 && score <= 30 turn on [In] [Active] ---> Channels 3,4
// 4- If score >= 31 && score <= 40 turn on [In] [Active] [Ish] ---> 3,4,5
// 5- If score >= 41 && score <= 50 turn on [Active] [Ish] ---> 4,5
// 6- If score >= 51 && score <= 70 turn on [Active] ---> 4
// 7- If score >= 71 && score <= 90 turn on [Super] [Active] [Ish] [!!!] ---> 2,4,5
// 8- If score >= 91 turn on [Super] [Active] [!!!] [circle] --- > 2,4,6 
// 9- If score == 0 nothing is on
// 10- Initializing state turn on all ---> channels 2,3,4,5,6

class TurnOn {
  int score;
  int arr[5];
  int pins[5];

  public:
      TurnOn(int value)
      {
        score = value;
      }

      int* getPin(){
        if (score >= 1 && score <= 10) // [Super] [In] [Active] [!!] ---> Channels 2,3,4
        {
          return writeToArray(2,3,4,0,0);
        }
        else if (score >= 11 && score <= 20) // [Super] [In] [Active] [Ish] [!!] ---> Channels 2,3,4,5
        {
          return writeToArray(2,3,4,5,0);
        }
        else if (score >= 21 && score <= 30) // [In] [Active] ---> Channels 3,4
        {
          return writeToArray(3,4,0,0,0);
        }
        else if (score >= 31 && score <= 40) // [In] [Active] [Ish] ---> 3,4,5
        {
          return writeToArray(3,4,5,0,0);
        }
        else if (score >= 41 && score <= 50) // [Active] [Ish] ---> 4,5
        {
          return writeToArray(4,5,0,0,0);
        }
        else if (score >= 51 && score <= 70) // [Active] ---> 4
        {
          return writeToArray(4,0,0,0,0);
        }
        else if (score >= 71 && score <= 90) // [Super] [Active] [Ish] [!!!] ---> 2,4,5
        {
          return writeToArray(2,4,5,0,0);
        }
        else if (score >= 91) // [Super] [Active] [!!!] [circle] --- > 2,4,6 
        {
          return writeToArray(2,4,6,0,0);
        }
        else if (score == 0)
        {
          return writeToArray(0,0,0,0,0);
        }
        else // if there is something wrong all the lights will be on
        {
          return writeToArray(2,3,4,5,6);
        }
      }

      int* writeToArray(int pin1, int pin2, int pin3, int pin4, int pin5){
        arr[0] = pin1;
        arr[1] = pin2;
        arr[2] = pin3;
        arr[3] = pin4;
        arr[4] = pin5;
        return arr;
      }

      int* copyArray(int * pin_out){
        for (int i = 0; i < 5; i++){
          pins[i] = pin_out[i];
        }
        return pins;
      }

      bool arrayIsSame(int * pins, int * temp_pins){
        for (int i = 0; i < 5; i++){

          Serial.print(pins[i]);
          Serial.print(" ");
          Serial.println(temp_pins[i]);
          Serial.println();

          if (pins[i] != temp_pins[i])
          {
            return false;
          }
        }
        return true;
      }
};
