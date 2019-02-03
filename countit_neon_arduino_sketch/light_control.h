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

// If score >= 1 && score <= 20 turn on [Super] [In] [Active] [!!] ---> Channels 2,3,4
// If score >= 21 && score <= 30 turn on [In] [Active] ---> Channels 3,4
// If score >= 31 && score <= 40 turn on [In] [Active] [Ish] ---> 3,4,5
// If score >= 41 && score <= 60 turn on [Active] [Ish] ---> 4,5
// If score >= 61 && score <= 90 turn on [Active] ---> 4
// If score >= 91 turn on [Super] [Active] [!!!] [circle] --- > 2,4,6 
// If score == 0 nothing is on
// Initializing state turn on all ---> channels 2,3,4,5,6


class TurnOn {
  float score;
  int* arr = new int[5];

  public:
      TurnOn(float value)
      {
        score = value;
      }

      int* getPin(){
        if (score >= 1 && score <= 20)
        {
          return writeToArray(4,3,2,0,0);
        }
        else if (score >= 21 && score <= 30)
        {
          return writeToArray(3,4,0,0,0);
        }
        else if (score >= 31 && score <= 40)
        {
          return writeToArray(3,4,5,0,0);
        }
        else if (score >= 41 && score <= 60)
        {
          return writeToArray(4,5,0,0,0);
        }
        else if (score >= 61 && score <= 90)
        {
          return writeToArray(4,0,0,0,0);
        }
        else if (score >= 91)
        {
          return writeToArray(2,4,6,0,0);
        }
        else if (score == 0)
        {
          return writeToArray(0,0,0,0,0);
        }
        else
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
};
