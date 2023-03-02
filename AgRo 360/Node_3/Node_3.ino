/*
 Lora Node3
https://www.electroniclinic.com/
 
*/
#include <SPI.h>              // include libraries
#include <LoRa.h>


// #define ss 15  //GPIO 15
// #define rst 16  //GPIO 16
// #define dio0 2  //GPIO 4

int soil = A0; 
float moisture_percentage;
 
String outgoing;              // outgoing message
 
byte msgCount = 0;            // count of outgoing messages
byte MasterNode = 0xFF;     
byte Node3 = 0xDD;
 
 
 
void setup() {
  Serial.begin(9600);                   // initialize serial
  pinMode(soil, INPUT);
  while (!Serial);

 
  Serial.println("LoRa Duplex"); 
 
 
  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
 
  Serial.println("LoRa init succeeded.");
}
 
void loop() {

  
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());

  int sensor_analog;
  sensor_analog = analogRead(soil);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );

}
 
void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(MasterNode);              // add destination address
  LoRa.write(Node3);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}
 
void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
 
  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
 
  String incoming = "";
 
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
 
  if (incomingLength != incoming.length()) {   // check length for error
   // Serial.println("error: message length does not match length");
   ;
    return;                             // skip rest of function
  }
 
  // if the recipient isn't this device or broadcast,
  if (recipient != Node3 && recipient != MasterNode) {
    //Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
    Serial.println(incoming);

    int Val = incoming.toInt();
    if(Val == 88)
    { 
      if(moisture_percentage<50.00){
        String message = String(moisture_percentage); 
        sendMessage(message,MasterNode,Node3);
        delay(100);
    }
      else if(moisture_percentage>50.00){
          String message = String("011"); 
          sendMessage(message,MasterNode,Node3);
            delay(100);
    }
    }
  
}