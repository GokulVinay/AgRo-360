/*
  Master Lora Node (ESP)
*/
#include <SPI.h>              // include libraries
#include <Wire.h>
#include <LoRa.h>

#define ss 15  //GPIO 15
#define rst 16  //GPIO 16
#define dio0 2  //GPIO 4
 
byte MasterNode = 0xFF;     
byte Node1 = 0xBB;
byte Node2 = 0xCC; 
byte Node3 = 0xDD; 
 
String SenderNode = "";
String outgoing;              // outgoing message
 
byte msgCount = 0;            // count of outgoing messages
 
// Tracks the time since last event fired
unsigned long previousMillis=0;
unsigned long int previoussecs = 0; 
unsigned long int currentsecs = 0; 
unsigned long currentMillis = 0;
int interval= 1 ; // updated every 1 second
int Secs = 0; 
 
 
void setup() {
  Serial.begin(115200);                   // initialize serial
  pinMode(5,OUTPUT);

  while (!Serial);
  Serial.println("LoRa Master Node");
  LoRa.setPins(ss, rst, dio0);
 
  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
}


 
void loop() {
  
currentMillis = millis();
   currentsecs = currentMillis / 1000; 
    if ((unsigned long)(currentsecs - previoussecs) >= interval) {
     Secs = Secs + 1;
    
     if ( Secs >= 16 )
    {
      Secs = 0; 
    }

    if ( (Secs >= 1) && (Secs <= 5) )
    {
     
    String message = "11"; 
    
    sendMessage(message,MasterNode, Node1);
    }
 
        if ( (Secs >= 6 ) && (Secs <= 10))
    {
     
    String message = "22"; 
    sendMessage(message,MasterNode, Node2);
    }
        if ( (Secs >= 11 ) && (Secs <= 15))
    {
     
    String message = "88"; 
    sendMessage(message,MasterNode, Node3);
    }
    
   previoussecs = currentsecs;
    }
 
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
    
  }
 
 
void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(otherNode);              // add destination address
  LoRa.write(MasterNode);             // add sender address
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
  if( sender == 0XBB )
  SenderNode = "Node1:";
  if( sender == 0XCC )
  SenderNode = "Node2:";
  if( sender == 0XDD )
  SenderNode = "Node3:";
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
 
  String incoming = "";
 
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
 
  if (incomingLength != incoming.length()) {   // check length for error
    //Serial.println("error: message length does not match length");
    ;
    return;                             // skip rest of function
  }
 
  // if the recipient isn't this device or broadcast,
  if (recipient != Node1 && recipient != MasterNode) {
   // Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
 
 //Node 1
 if( sender == 0XBB )
 { 
   Serial.println("Node 1");
   Serial.println(incoming+" val");
 }


//Node2
  if( sender == 0XCC )
 {
   Serial.println("Node 2");
   Serial.println(incoming + "C");
 }

 
//Node 3
  if( sender == 0XDD )
  { 

    Serial.println("Node 3");
    Serial.println(incoming + "%");
    if(incoming=="011"){
      digitalWrite(5,HIGH);
    } 
    else{
      digitalWrite(5,LOW);
    }

     }


delay(1000);
}



