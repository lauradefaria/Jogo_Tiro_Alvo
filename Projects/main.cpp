#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include "SerialClass.h"	// Library described above
#include <string>

using namespace std;

// application reads from the specified serial port and reports the collected data
int main()
{
	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("\\\\.\\COM6");    // adjust as needed
	
	if (SP->IsConnected())
		printf("We're connected");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 255;
	
	int readResult = 0;
	
	/*if(0){
			
	}
	else{                                    //QUANDO O OPENCV NÃO ESTIVER RODANDO
		
		int pontos, placar, valorInt=0;
		char valorStr[10];
	
		ifstream
        	input("placar.txt", ios::in);
	    ofstream
	        output("placar.txt", ios::out | ios::trunc);
	
		
	    if(!input.is_open()){
	        return 1;
	    }
	    if(!output.is_open()){
	        return 1;
	    }
	
		while(SP->IsConnected())
		{
	    
			readResult = SP->ReadData(incomingData,dataLength);
			// printf("Bytes read: (0 means no data available) %i\n",readResult);
			incomingData[readResult] = 0;
			valorInt = atoi(incomingData);
			cout << valorInt << endl; 
	
			Sleep(500);
			
			
			while(!input.eof()){
		
	    		// getline(input, pontos);
	    		input >> pontos;
  			  	  cout << "PONTOS: " << pontos << endl;
		    	if(valorInt > pontos){ 
		        	output << valorInt << endl;
		        	placar = valorInt;
				}
				else{
					placar = pontos;
				}
			}
			
			itoa(placar,valorStr,10); 
			
			output.close();
			input.close();*/
	
			SP->WriteData("283", 11);
			pri;
		}
	//}
	
	return 0;
}