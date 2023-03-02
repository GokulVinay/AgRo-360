/*
 Lora Node1(ARD)
*/
#include <SPI.h>              // include libraries
#include <Wire.h>
#include <LoRa.h>
#include <DHT.h>

#define DHTPIN 2
DHT dht(DHTPIN, DHT11);
 
int POT = A0; 
 
String outgoing;  
float t;            // outgoing message
 
byte msgCount = 0;            // count of outgoing messages
byte MasterNode = 0xFF;     
byte Node2 = 0xCC;
 
 
void setup() {
  Serial.begin(9600); 
  dht.begin();                  // initialize serial
  pinMode(POT, INPUT);
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

  float h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) 
  {
  Serial.println("Failed to read from DHT sensor!");
  return;
  }
  // Serial.print("Temperature: ");
  // Serial.print(t);
 
 

}
 
void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(MasterNode);              // add destination address
  LoRa.write(Node2);             // add sender address
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
  if (recipient != Node2 && recipient != MasterNode) {
    //Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
    Serial.println(incoming);
    int Val = incoming.toInt();
    if(Val == 22)
    { 
    String message = String(t); 
    sendMessage(message,MasterNode,Node2);
    delay(100);
    }
  
}
