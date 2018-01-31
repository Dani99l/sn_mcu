/*
*   Set nodemcu with RF95 LoRa modem to send data to gateway
*   Open source libraries form libelium
*   Daniela Lourenco
**/

/*Important Informations about this code */

/* -> The gateway and sensor nodes must be configured with the same configurations (channel, sf, cr, bw) (it was selected randomly)
 *  -> HEADER 
 *      -It is possible to configure EXPLICIT or IMPLICIT mode to header packets (**Must take into consideration both nodes**)
 *      -IMPLICIT mode (  header must be configured manually) -> (ATTENTION - Only possible to use SF=6 with this configuration)
 *      -EXPLICIT mode ( (preamble bits + CRC + payloadLenghtCRC) configured automatically)
 */     
/*Configuarations:
 *   setmode(10) -> only for testing with the biggest BW possible (Importante to test with the different modes to verify the reach)
 *   Set Frequency channel to 868 MHz (Channel 17)
 *   Set Power to 14 dBm
 *   SF=7
 *   CR=5
 *   BW=500kHz
 */

/******************************************************************************
 * Definitions & Declarations
 *****************************************************************************/
#define ADDR_NODE 1   //Packet destination
char message[10]="test_123";
uint8_t packet_sum;
#include "_SX1272.h"


void setup() {

  initSerial();
  Serial.println("Testing sensor node");
  Serial.println("");
  sx1272.CONFIGMODEM(); //Initialize variables -> check configurations in _SX1272.cpp
  configRadio();
}
    

void initSerial(){
  
  delay(300); //adviced to add a delay in serial communications
  Serial.begin(115200);
  
}

void configRadio(){
  /*Set mode, frequency_channel, output_power, node_address, spreading_factor, header */
    
    uint8_t e;
 
    sx1272.ON(); 
    
    
    e=1;
    while(e!=0){
    
       e = sx1272.setMode(1);  // working with mode 1 ....

    }
    
    e=1;
    while(e!=0){
      e=sx1272.setChannel(CH_18_868);
    }
    
    e=1;
    while(e!=0){
      e=sx1272.setPower('M', 'x');
      if(e==0){
           Serial.println(F("Set power to 14 dbm"));
      }
    }    
    
    e=1;
    while(e!=0){
      e=sx1272.setNodeAddress(ADDR_NODE);
    }

    delay(500);
}

void loop() {
  //variables debug
  
    long startSend;
    long endSend;
    uint8_t e;
    
    packet_sum++;
    
    if(packet_sum>250){
      packet_sum=0;
    }
    
    sprintf(message,"%d", packet_sum);

    Serial.println("Preparing to send: ");
    Serial.write(message);
    Serial.println("Payload: ");
    Serial.print(sizeof(message));
    e=0;
   // startSend = millis();
     e=sx1272.sendPacketTimeout(5, message);
  //  endSend = millis();
     
    if(e==0){
      Serial.println("Message sent ");   

      for(int i=0; i<sx1272._payloadlength; i++){
         Serial.print(sx1272.packet_sent.data[i], DEC);
         Serial.print(" ");
      }

      Serial.println("");

    }
    else{
      Serial.println("ERROR - Packet not sent: ");
      Serial.print(packet_sum);
    }
    
    Serial.println(" ");

//    Serial.println("Entering to Sleep mode");
//    sx1272.setSleepMode(); 

    /*Add a delay between packets sent */

    delay(8000);
    sx1272.setSleepMode();
    delay(8000);

//    sx1272.getBW();
//    sx1272.getSF();
//    sx1272.getCRC();
//    sx1272.getHeader();
    /*Need to set up again initial configuration -> configRadio()*/
//   configRadio();
}
