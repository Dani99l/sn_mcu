
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
// Include the SX1272

#include "SX1272.h"   //using RF95 modem
#include <EEPROM.h>

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


#define PRINTLN                   Serial.println("")
#define PRINT_CSTSTR(fmt,param)   Serial.print(F(param))
#define PRINT_STR(fmt,param)      Serial.print(param)
#define PRINT_VALUE(fmt,param)    Serial.print(param)
#define FLUSHOUTPUT               Serial.flush();

#define DEFAULT_DEST_ADDR 1


unsigned long delayBeforeTransmit = 0;
char float_str[20];
uint8_t message[10]="123456789";
int loraMode = LORAMODE;
uint8_t packetNumber;

//struct sx1272config {
//
//  uint8_t flag1;
//  uint8_t flag2;
//  uint8_t seq;
//  // can add other fields such as LoRa mode,...
//};

//sx1272config my_sx1272config;

uint8_t e;

void setup()
{
  
  Serial.begin(115200);
  PRINT_CSTSTR("%s", "TESTE\n");

  // Power ON the module
  sx1272.ON();

  // Set transmission mode and print the result
  e = sx1272.setMode(loraMode);                    //init lora module
  PRINT_CSTSTR("%s", "Setting Mode: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;

  // enable carrier sense
  //sx1272._enableCarrierSense = true;

#ifdef BAND868
  // Select frequency channel
  e = sx1272.setChannel(CH_18_868);
#endif
  PRINT_CSTSTR("%s", "Setting Channel: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;

  // Select output power (Max, High or Low)
#if defined RADIO_RFM92_95  || defined RADIO_INAIR9B
  e = sx1272.setPower('x'); //14dBm
#else
  e = sx1272.setPower('M');
#endif

  PRINT_CSTSTR("%s", "Setting Power: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;

  // Set the node address and print the result
  e = sx1272.setNodeAddress(node_addr);
  PRINT_CSTSTR("%s", "Setting node addr: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;

//  // Print a success message
//  PRINT_CSTSTR("%s", "SX1272 successfully configured\n");

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
  uint8_t app_key_offset = 0;
  int e;

  uint8_t r_size;
    // messsage[0]=packetNumber;
     packetNumber++;
    //PRINT_CSTSTR("%s", "Sending ");
    PRINT_STR("%s",(char*)(message));
   PRINT_STR("%d" ,packetNumber);

    PRINTLN;
    r_size=sizeof(message);
    PRINT_CSTSTR("%s", "Real payload size is ");
    PRINT_VALUE("%d", r_size);
    PRINTLN;

    int pl = r_size;

   // sx1272.CarrierSense();

    startSend = millis();

    // Send message to the gateway and print the result
//    message[0]=packetNumber;
//    message[1]=5;

    e = sx1272.sendPacketTimeout(DEFAULT_DEST_ADDR, message, pl);

    endSend = millis();


    // save packet number for next packet in case of reboot
   // my_sx1272config.seq = sx1272._packetNumber;

    PRINT_CSTSTR("%s", "LoRa Sent in ");
    PRINT_VALUE("%ld", endSend - startSend);
    PRINTLN;

    PRINT_CSTSTR("%s", "Packet sent, state ");
    PRINT_VALUE("%d", e);
    PRINTLN;

    // use a random part also to avoid collision
   // PRINT_VALUE("%ld", lastTransmissionTime);
    PRINTLN;
    PRINT_CSTSTR("%s", "Will send next value in\n");
  //  lastTransmissionTime = millis();
    delayBeforeTransmit = idlePeriodInMin * 60 * 1000 + random(15, 60) * 1000;
    PRINT_VALUE("%ld", delayBeforeTransmit);
    PRINTLN;

    delay(500);    
    delay(500);
    delay(500);

  }



