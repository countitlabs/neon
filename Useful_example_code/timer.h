// needs the following library elapsedMilis v 1.0.4

#include <elapsedMillis.h>

elapsedMillis timeElapsed; //declare global if you don't want it reset every time loop runs

void setup() 
{                
  Serial.begin(9600);
}

void loop()
{
  Serial.print("New time: ");
  Serial.println(timeElapsed);

  if (timeElapsed > 5000)
  {
    Serial.println("5 secods have passed!");
  }
  else if (timeElapsed > 10000)
  {
    Serial.println("10 seconds have passed!");
  }
}


//300000 = 5 minutes

//180000 = 3 minutes