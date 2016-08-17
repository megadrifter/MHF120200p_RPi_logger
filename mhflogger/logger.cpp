// all the startup stuff with headers
#include <cstdlib>
#include <iostream>
// later #include <fstream> 

#include "../RF24.h"

using namespace std;

// gpio pins, spi speed - left unchanged, should work
RF24 radio("/dev/spidev0.0",8000000, 22");

int main(int argc, char** argv)
{


/** Setup mhf **/
radio.begin();
radio.setDataRate( RF24_2MBPS);
radio.setChannel(40);
radio.openReadingPipe(0,0x0110104334);
radio.setCRCLength( RF24_CRC_16 ) ; // already set
// how to close the pipe 1?
// пока не буду (tx_addr)
radio.setPayloadSize(32); // initially set
// radio.enableAckPayload(); // пока не надо
// radio.setAutoAck(1); // 
// radio.enableDynamicPayloads(); // пока не надо

radio.startListening(); // ПОЕХАЛИ! ~O+=
/* this seems to be enough. */ 

/** set up variables **/
char receivePayload[33]; // container for payload

radio.printDetails(); // see all settings
cout<<"ПОЕХАЛИ! ~O+= \n";

// waiting (or not) for cought packets k times and printing them
for(int k=0 ; k<10;k++) 
  { // will do k attempts instead of while(1)
    delay(1000); // Data is transmitted 5 times/ sec. Wait plenty of time.
    
    // if (radio.available()) // temporarily blocked. will read in any case 
    {
      /** Clear measurement values **/ 
      memset(receivePayload,0,32);
      
      radio.read(receivePayload, 32);
    
      /** print payload bytes separatelly in decimal format **/
      for (int digit=0; digit < 32; ++digit)
          printf("%d ", (int)(receivePayload[digit])); 
      cout <<"\n";radio.printDetails();
    }
  }

printf("Finished\n");  

return 0;
}

