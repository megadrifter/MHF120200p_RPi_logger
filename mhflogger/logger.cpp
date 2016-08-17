/* TODO 
запуск и остановка по <???>
при стопе запись остатков
формат данных: csv
класс mhfLogger? (начать в отдельном файле .н)
*/

// all the startup stuff with headers
#include <cstdlib>
#include <iostream>
#include <fstream> 
#include <ctime>

#include <RF24/RF24.h> // use installed library

using namespace std; // for cout

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

int main(int argc, char** argv)
{
/* при старте создаётся новый лог-файл, в имени - время (как треки гпс) */
/** prepare the name of current file, including name (path and name as parameter?) with date **/

char nowtime[32];
time_t t = time(0);
strftime(nowtime, sizeof(nowtime), "%Y-%m-%d-%H-%M-%S", localtime(&t));
cout << nowtime << '\n';

char* file_name = /*nowtime*/ "mhflog.csv"; // формат год-месяц-день-час-мин-сек-mhflog.csv
/*FILE *file;
file = fopen( file_name, "w" ); 
fputs( "Begin \n", file );
fclose( file );*/
ofstream file;  
file.open ("Test.txt", ios::out);
file << "Begin \n"; // << endl
      file  << "BpemR"  << ";"
            <<  "V" << ";"
            <<  "A" << ";
            <<  "H" << ";"
            <<  "Ah" << "\n";
file.close;


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
int bufrows = 900; // how many rows in the buffer
char receivePayload[bufrows][33]; // container for payload + "\0"
time_t timesec[bufrows];

radio.printDetails(); // see all settings
cout<<"ПОЕХАЛИ! ~O+= \n";


// waiting (or not) for cought packets k times and printing them
while(1)
  { 
    for (int i=0; i<bufrows; ) // counter if recieved
    {
      delay(90); // Data is transmitted 5 times/ sec. Wait some time.
      // вставить обработчик завершения?
      
      if (radio.available())
      {
        /** Clear measurement values **/ 
        memset(receivePayload,0,32);
        /** write to a memory buffer **/ 
        radio.read(receivePayload[i], 32);
        
        // напечатать дату и время
        // cout << date[i] = ;
        timesec[i] = time(0);
        
        /** print payload bytes separatelly in decimal format **/
        for (int digit=0; digit < 32; ++digit)
            printf("%d ", (int)(receivePayload[i][digit])); 
        cout <<"\r"; // make a single row
        ++i; 
      }
      
    }
    /** append to a file  **/
    /* file = fopen( file_name, "a" ); 
    char thestring = receivePayload[i];
    fputs( thestring, file );*/
    file.open (file_name, ios::app);
    for (int i=0; i<bufrows; ++i) 
    {
      strftime(nowtime, sizeof(nowtime), "%Y-%m-%d-%H-%M-%S", localtime(&timesec[i]));
      file  <<  nowtime << ";"
            <<  receivePayload[i][1] << ";"
            <<  receivePayload[i][3] << ";"
            <<  receivePayload[i][5] << ";"
            <<  receivePayload[i][7] << "\n";
    }
    
    file.close;
    
  }

printf("Finished\n");  

return 0;
}

