#include <Arduino.h>

void setup()
{
  // put your setup code here, to run once:
  pinMode(9, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(9, 1);
  delay(100);
  digitalWrite(9, 0);
  delay(100);
}