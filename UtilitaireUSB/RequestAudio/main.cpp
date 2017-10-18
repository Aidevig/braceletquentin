#include <fstream>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include "Serial.h"	// Library described above
#include <string>
#include <sstream>

// application reads from the specified serial port and reports the collected data
int _tmain(int argc, _TCHAR* argv[])
{
    //PORT COM SCANNER A METTRE DANS UNE NOUVELLE CLASS
    int counter=0;
    int i,numFile=0;
    HANDLE hComm;
    std::string UniName = "\\\\.\\COM";
    std::string pcCommPort;
    std::stringstream temp;

for (counter = 1; counter <=99; counter++) {
    temp.str("");
    temp << UniName << counter;
    pcCommPort = temp.str();

   //std::cout<<pcCommPort.c_str()<<std::endl;
    hComm = CreateFile(pcCommPort.c_str(),
                          GENERIC_READ | GENERIC_WRITE,
                          0,    // comm devices must be opened w/exclusive-access
                          NULL, // no security attributes
                          OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                          0,    // non overlapped I/O
                          NULL  // hTemplate must be NULL for comm devices
                          );

        if(hComm == INVALID_HANDLE_VALUE)
        {
            if(GetLastError()==ERROR_FILE_NOT_FOUND){
               // std::cout<<"Doesn't Exist"<<std::endl;
            }
            else{
                    // std::cout<<"Others"<<std::endl;
            }
            CloseHandle(hComm);
        }
        else{
   //std::cout<<pcCommPort<<std::endl;

            CloseHandle(hComm);
   break;
        }
    }
    ///FIN CLASS SCANNER

	printf("Welcome to the serial test app!\n\n");
	Serial* SP = new Serial(pcCommPort.c_str());    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected\n\n");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 255;
	char command[1];
	char nameShort[9];
	char nameMed[10];
	char nameLong[10];
	command[0] = 'A';
	int readResult = 0;
    std::ofstream myfile;

	if(SP->IsConnected())
	{
	    printf("Press a Enter to start...");
	    getchar();
	    SP->WriteData(command,1);
	    Sleep(500);
	    if(SP->Available()){
            readResult = SP->ReadData(incomingData,dataLength);
            // printf("Bytes read: (0 means no data available) %i\n",readResult);
                    incomingData[readResult] = 0;

                //printf("%s",incomingData);
                numFile = atoi(incomingData);
                Sleep(50);
	    }
	    printf("number of files: %d\n", numFile);
        for(i=0;i<numFile;i++){
            printf("Fichier %d:\n",i);
            command[0]='N';
            SP->WriteData(command,1);
            Sleep(500);
            //if(SP->Available()){
                readResult = SP->ReadData(incomingData,dataLength);
                // printf("Bytes read: (0 means no data available) %i\n",readResult);
                        incomingData[readResult] = 0;

                printf("%d\n",sizeof(incomingData));
                Sleep(3);
            //}
           /* if(i<10){
            strncpy(nameShort, incomingData, 8);
            nameShort[8]='\0';
           }
            else if(i<100){
                strncpy(nameMed, incomingData, 9);
                }*/
            ///ATTENTION DECOMENTER LES LIGNES AU DESSUS ET COMMENTER CELLE EN DESSOUS!!!!
            strncpy(nameMed, incomingData, 9);
            nameMed[9]='\0';
            ///
            printf("%s", nameMed);
            myfile.open(nameMed);
            if(!myfile.is_open()){ printf("Probleme fichier");}
            else{
                printf("%s Opened\n",nameMed);
                command[0]='O';
                SP->WriteData(command,1);
                Sleep(500);
                while(SP->Available()){
                    readResult = SP->ReadData(incomingData,dataLength);
                    //printf("Bytes read: (0 means no data available) %i\n",readResult);
                            incomingData[readResult] = 0;

                        printf("%s",incomingData);
                        myfile.write(incomingData,readResult);
                    Sleep(5);
                }
                myfile.close();
            }
	    }
	    /*myfile.open("dataWav.wav");
	    if(!myfile.is_open()) printf("boloss");

	    while(SP->Available()){
            readResult = SP->ReadData(incomingData,dataLength);
            // printf("Bytes read: (0 means no data available) %i\n",readResult);
                    incomingData[readResult] = 0;

                printf("%s",incomingData);
                myfile.write(incomingData,readResult);
            Sleep(3);
	    }
	    myfile.close();*/
	}
	return 0;
}
