//File: pwm_example.cpp
//Description: Sends a HIGH signal for 10 seconds and changes it to a LOW signal for 10 seconds 
// FOR MKR1000

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  Serial.println("Starting High");
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(HIGH);
  delayMicroseconds(10000000); // 10 Khz aprox to 10 seconds delay
  Serial.println("Starting Low");
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(LOW);
  delayMicroseconds(10000000);
}


// This part of the code allows us to send HIGH and LOW signals to leds in parallel
// as an example of how we could control the power supplies
// 1,000,000 hz == 1 second delay
// 10,000,000 hz == 10 sencond delay
// http://www.circuitbasics.com/wp-content/uploads/2014/12/Basic-LED.png

void setup()
{
  Serial.begin(9600);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop()
{
  Serial.println("Starting High");
  digitalWrite(0, HIGH);
  delay(1000);
  digitalWrite(1, HIGH);
  delay(1000);
  digitalWrite(2, HIGH);
  delay(1000); // 1 khz aprox to 1 seconds delay
  digitalWrite(3, HIGH);
  delayMicroseconds(10000000); // 10 khz aprox to 10 seconds delay
  Serial.println("Starting Low");
  digitalWrite(0, LOW);
  delay(1000);
  digitalWrite(1, LOW);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
  digitalWrite(3, LOW);
  delay(1000);
}

// -----------------------------------------------------------
// For ARDUINO UNO
// Note the pins are different and we are using delay instead of microdelay

void setup()
{
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop()
{
  Serial.println("Starting High");
  digitalWrite(7, HIGH);
  delay(1000);
  digitalWrite(6, HIGH);
  delay(1000);
  digitalWrite(5, HIGH);
  delay(1000); // 1 khz aprox to 1 seconds delay
  digitalWrite(4, HIGH);
  delay(10000); // 10 khz aprox to 10 seconds delay
  Serial.println("Starting Low");
  digitalWrite(7, LOW);
  delay(1000);
  digitalWrite(6, LOW);
  delay(1000);
  digitalWrite(5, LOW);
  delay(1000);
  digitalWrite(4, LOW);
  delay(1000);
}


// -----------------------------------------------------------
// Example for 5 channels arduin uno wifi rev 2

void setup()
{
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop()
{
  Serial.println("Starting High");
  digitalWrite(7, HIGH);
  delay(1000);
  digitalWrite(6, HIGH);
  delay(1000);
  digitalWrite(5, HIGH);
  delay(1000); // 1 khz aprox to 1 seconds delay
  digitalWrite(4, HIGH);
  delay(10000); // 10 khz aprox to 10 seconds delay
  Serial.println("Starting Low");
  digitalWrite(7, LOW);
  delay(1000);
  digitalWrite(6, LOW);
  delay(1000);
  digitalWrite(5, LOW);
  delay(1000);
  digitalWrite(4, LOW);
  delay(1000);

