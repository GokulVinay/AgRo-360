

#include <Wire.h>



#define Rs_pin  0

#define led_pin 13

void setup()
{
  Serial.begin(9600);
  
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
}

void loop()
{
  printVolts();
}
 
 void printVolts()
{
  int sensorValue = analogRead(A0); //read the A0 pin value
  float voltage = sensorValue * (4.60 / 1023.00) * 2; //convert the value to a true voltage.
  Serial.print("voltage = ");
  Serial.print(voltage); //print the voltage to LCD
  Serial.print(" V");
  Serial.println(" ");
  delay(4000);
  if (voltage < 6.50) //set the voltage considered low battery here
  {
    digitalWrite(led_pin, HIGH);
  }
}