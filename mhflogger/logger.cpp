// all the startup stuff with headers
#include <cstdlib>
#include <iostream>
// later #include <fstream> 

#include <RF24/RF24.h> // use installed library

using namespace std; // for cout

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

int main(int argc, char** argv)
{


/** Setup mhf **/
radio.begin();
radio.setDataRate( RF24_2MBPS );
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
char receivePayload[33]; // container for payload + "\0"

radio.printDetails(); // see all settings
cout<<"ПОЕХАЛИ! ~O+= \n";

// waiting (or not) for cought packets k times and printing them
while(1)
  { 
    delay(90); // Data is transmitted 5 times/ sec. Wait some time.
    
    if (radio.available())
    {
      /** Clear measurement values **/ 
      memset(receivePayload,0,32);
      radio.read(receivePayload, 32);
      /** print payload bytes separatelly in decimal format **/
      
      for (int digit=0; digit < 32; ++digit)
          printf("%d ", (int)(receivePayload[digit])); 
      cout <<"\r"; // make a single row
      
      /** write to a memory buffer **/ 
    
    }
    
      /** append to a file if size is...  or time interval...  **/
      
    
  }

printf("Finished\n");  

return 0;
}

