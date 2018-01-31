
/*
    temperature sensor on analog 8 to test the LoRa gateway

    Copyright (C) 2016 Congduc Pham, University of Pau, France

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the program.  If not, see <http://www.gnu.org/licenses/>.

 *****************************************************************************
   Adapted for ESP8266 modules by Marcio Miguel - marcio.miguel@gmail.com
 *****************************************************************************

 ****************************************************************************
 ****************************************************************************
  Adapted to work as a sensor node LoRa to send random information to
  gateway LoRa
  By Daniela LourenÃƒÂ§o
 *****************************************************************************
 *****************************************************************************

*/
//#include <SPI.h>
#include "SX1272.h"   //using RF95 modem

// IMPORTANT
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// please uncomment only 1 choice
//
#define RADIO_RFM92_95
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// IMPORTANT
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// please uncomment only 1 choice

#define BAND868


#define LORAMODE  1

#define node_addr 5

/////////////////////////////////////////////////////////////////// Not used (Adpating code)(DL)*
// CHANGE HERE THE THINGSPEAK FIELD BETWEEN 1 AND 4
#define field_index 2
///////////////////////////////////////////////////////////////////

unsigned int idlePeriodInMin = 2;
String my_packet_str;


#define PRINTLN                   Serial.println("")
#define PRINT_CSTSTR(fmt,param)   Serial.print(F(param))
#define PRINT_STR(fmt,param)      Serial.print(param)
#define PRINT_VALUE(fmt,param)    Serial.print(param)
#define FLUSHOUTPUT               Serial.flush();

#define DEFAULT_DEST_ADDR 1


unsigned long delayBeforeTransmit = 0;
char float_str[20];
char message[10]="test_123";
int loraMode = LORAMODE;
uint8_t packetNumber;
uint8_t e;
String mes;
void setup()
{
  delay(3000);
  Serial.begin(115200);
  PRINT_CSTSTR("%s", "TESTE\n");

  // Power ON the module
  sx1272.ON();

  // Set transmission mode and print the result
  e = sx1272.setMode(loraMode);                    //init lora module
  PRINT_CSTSTR("%s", "Setting Mode: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;
  e=1;
  // Radio configurations
  while(e!=0){
    e = sx1272.setChannel(CH_18_868);
    if(e==0){
      Serial.print(F("Set Channel to 868.1 MHz"));
    }
  }
 
  e=1;
  while(e!=0){
      e = sx1272.setPower('x'); //14dBm
    if(e==0){
      Serial.println(F("Set power to 14 dbm"));
    }
  }
  
  e=1;
  while(e!=0){
      e = sx1272.setMode(10);  //Set mode 10
      if(e==0){
        Serial.println(F("Set CR=5, SF=7, BW=500kHz"));
      }
  }
  
   e=1;
  while(e!=0){
      e = sx1272.setNodeAddress(node_addr);
      if(e==0){
        Serial.println("Set node address: ");
        Serial.print(node_addr);
      }
  }
  delay(500);
}


char *ftoa(char *a, double f, int precision)
{
  long p[] = {0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}


void loop(void)
{
    
    long startSend;
    long endSend;
    int e;
    uint8_t r_size;
    char messageUpdate[10];
    if(packetNumber>250){
      packetNumber=0;
    }
    message[0]='\0';
    sprintf(message,"test_%d", packetNumber);
  //  strcat(messageUpdate,message);
    packetNumber++;

    Serial.println("Preparing to send: ");
  //  Serial.write(messageUpdate);
    Serial.write(message);
    
    PRINTLN;
    PRINT_STR(" Packet Number %d" ,packetNumber);
    PRINTLN;
    r_size=sizeof(message);
    PRINT_CSTSTR("%s", "Packet payload size is ");
    PRINT_VALUE("%d", r_size);
    PRINTLN;

    int pl = r_size;

    startSend = millis();
//    e = sx1272.sendPacketTimeout(node_addr, messageUpdate);
     e = sx1272.sendPacketTimeout(node_addr, message);
    endSend = millis();


    PRINT_CSTSTR("%s", "LoRa Sent in ");
    PRINT_VALUE("%ld", endSend - startSend);
    PRINTLN;
    Serial.println("Payload lenght: ");
    Serial.println(sx1272._payloadlength);
    if(e==0){
      PRINT_CSTSTR("%s", "Packet sent with success! ");
      PRINT_VALUE("%d", e);
      PRINTLN;
          for(int i=0; i<sx1272._payloadlength; i++){
              Serial.print(sx1272.packet_sent.data[i], DEC);
              Serial.print(" ");
           }
          Serial.println("");

    }
    else{
      Serial.println("Sending Packet ERROR!");
    }
    
    PRINTLN;
    PRINT_CSTSTR("%s", "Will send next value in 4s \n");
    PRINTLN;
    
    sx1272.setSleepMode();

    delay(500);    
    delay(500);
    delay(500);
    delay(500);
    delay(500);    
    delay(500);
    delay(500);
    delay(500);
 }



