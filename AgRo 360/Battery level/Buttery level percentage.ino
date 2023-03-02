#include <Wire.h>
// #include <LcdBarGraphRobojax.h>
int offset =20;
byte sensorPin = 0; // -- value for this example
        
void setup(){
  Serial.begin(9600);
  Serial.print("Battery Voltage");  
  delay(2000);
}

void loop()
{
  int inpuValue = analogRead(A0);
  int volt = analogRead(A0);
  double voltage = map(volt,0,1023, 0, 2500) + offset;// map 0-1023 to 0-2500 and add correction offset
  voltage /=12.8;// divide by 100 to get the decimal values
  // -- draw bar graph from the analog value read
  // lbg.drawValue( inpuValue, 520);
  // -- do some delay: frequent draw may cause broken visualization
  Serial.print("Bat Charge:"); 
  Serial.print(voltage); // print
  Serial.print("%"); 
  Serial.println(" ");  
 
  delay(3000);
}
