
/*  Gateway LoRa configuration -> check sensor node code
*   Set nodemcu with RF95 LoRa modem to send data to gateway
*   Open source libraries from libelium
*   Daniela Lourenco
**/

#include "_SX1272.h"
#include <ESP8266WiFi.h>
const char* ssid = "Connectify-nodeteste";
const char* password = "nodeteste123";
#define led 2
#define _modem = LORA
#define _nodeAddress 1
boolean led_state = LOW;

int nCnt;
void setup() {
    int nCnt = 0;
    pinMode(led, OUTPUT);
    int e=0;
    initSerial();
    Serial.println("Testing gateway");
    initwifi();
    sx1272.CONFIGMODEM(); //Initialize variables -> check configurations in _SX1272.cpp
    configRadio();
}

void initSerial(){
  
  delay(1000); //adviced to add a delay in serial communications
  Serial.begin(115200);
   delay(1000); //adviced to add a delay in serial communications

}

void initwifi(){
    WiFi.begin(ssid, password);
   
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
      nCnt ++;
      if(nCnt > 10) break;
    }

    if(WiFi.status() == WL_CONNECTED){
      Serial.println("");
      Serial.println("WiFi connected");
    }
}

void configRadio(){
    /*Set mode, frequency_channel, output_power, node_address, spreading_factor, header */
    sx1272.ON();     

    sx1272.setMode(10);   

    
    sx1272.setHeaderON();

    sx1272.setChannel(CH_18_868);

    sx1272.setPower('M');
   // sx1272.setPowerNum(9);

    sx1272.setNodeAddress(_nodeAddress);


}

void loop() {
    int e;
   // e = sx1272.receiveAll(10000);
   // e=sx1272.receivePacketTimeout (10000);
   
   while(!sx1272.availableData()){
       e= sx1272.receiveAll(1000);
   }
    if(e == 0){
      Serial.println("Packet was received");
      Serial.println("Show Received Packet");
      sx1272.showReceivedPacket();
      Serial.println("Show Rx registers");
      sx1272.showRxRegisters();
      Serial.println("Show frame from packet");
      sx1272.showFramefromPacket();
      
      led_state = !led_state ;

      sx1272.getRSSIpacket();
      sx1272.getSNR();
     
    }
    else{
      Serial.println("NO Received Packet");
    }

   // delay(1000);
    
   // delay(1000);
    sx1272.getBW();
    sx1272.getSF();
    sx1272.getCRC();
    sx1272.getHeader();
    sx1272.clearFlags();
    
}
