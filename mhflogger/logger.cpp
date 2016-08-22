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

void twobytestoints(char byte1, char byte2, int* Ugreat, int* Usmall)
{	
	int Usmall_temp = (int)(byte1);
    int Ugreat_temp = (int)(byte2<<8);
    int U= Usmall_temp + Ugreat_temp; 
    *Ugreat =  U/100;
	*Usmall = U - *Ugreat*100;
	return;
}

int main(int argc, char** argv)
{
/* при старте создаётся новый лог-файл, в имени - время (как треки гпс) */
/** prepare the name of current file, including name (path and name as parameter?) with date **/

char nowtime[32];
time_t t = time(0);
strftime(nowtime, sizeof(nowtime), "%Y-%m-%d_%H-%M-%S", localtime(&t));
cout << nowtime << '\n';

char file_name[58] = "/home/pi/nRF24_logger/logs/";
strcat(file_name,"mhflog_");
strncat(file_name,nowtime,19);
strcat(file_name,".csv");

// формат год-месяц-день-час-мин-сек-mhflog.csv

/*FILE *file;
file = fopen( file_name, "w" ); 
fputs( "Begin \n", file );
fclose( file );*/
ofstream file;  
file.open (file_name, ios::out);
// file << "Begin \n"; // << endl
      file  << "i" << ";"<< "Timesec"<< ";"<< "BpemR"  << ";"
            <<  "V" << ";"
            <<  "A" << ";"
            <<  "H" << ";"
            <<  "Ah" << "\n";
file.close();


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
int bufrows = 600;// ~10 times per second for delay ~100  (depends on delay) ; // how many rows in the buffer
char receivePayload[bufrows][33]; // container for payload + "\0"
time_t timesec[bufrows];

radio.printDetails(); // see all settings
cout<<"ПОЕХАЛИ! ~O+= \n";


// waiting (or not) for cought packets k times and printing them
while(1)
  { 
	int Agr, Asm; // для отображения цифр до и после запятой
    
	for (int i=0; i<bufrows; ) // counter if recieved
  
 {
 	  //***** try without delay.   */
      delay(95); // Data is transmitted 5 times/ sec. Wait some time. 
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
			cout  << i << ") "<<  nowtime ;

			printf(" %d ", (int)(receivePayload[i][0])); 
			/** print known two-bytes payload data **/
			for (int k=1; k<9; k=k+2)
			{
				twobytestoints(receivePayload[i][k],receivePayload[i][k+1],&Agr,&Asm);
				cout << " " << Agr <<","; if (Asm<10) cout<<"0"; cout << Asm ;
			}

			for (int digit=9; digit < 32; ++digit)
            printf(" %d ", (int)(receivePayload[i][digit])); 
			cout <<"\r"; // make a single row
			++i; 
		}
      
    }
    /** append to a file  **/
    /* file = fopen( file_name, "a" ); 
    char thestring = receivePayload[i];
    fputs( thestring, file );*/
    cout << "\n writing to file " << file_name << "\n";
    file.open (file_name, ios::app);
    for (int i=1; i<bufrows; ++i) 
    {
      strftime(nowtime, sizeof(nowtime), "%Y-%m-%d %H:%M:%S", localtime(&timesec[i]));
      	  
	  file << i << ";" << timesec[i] << ";"<<  nowtime ;
		for (int k=1; k<9; k=k+2)
		{
			twobytestoints(receivePayload[i][k],receivePayload[i][k+1],&Agr,&Asm);
			file << ";" << Agr <<","; if (Asm<10) file<<"0"; file << Asm ;
		}
		/* and undecoded tail */
		for (int digit=9; digit < 32; ++digit)
            file << ";" << (int)(receivePayload[i][digit]); 
	  file << "\n";
    }
    
    file.close();
    
  }

printf("Finished\n");  

return 0;
}
