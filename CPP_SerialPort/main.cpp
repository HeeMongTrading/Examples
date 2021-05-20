

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rs232.h"


const int BUFSIZE=140;
wchar_t COMPORT_Rx[] =L"COM3";
wchar_t COMPORT_Tx[] =L"COM4";

HANDLE hComRx;
HANDLE hComTx;
int nComRate=9600;
int nComBits=8;
COMMTIMEOUTS timeout;

int main()
{
    initPort(&hComRx, COMPORT_Rx, nComRate, nComBits, timeout);
    Sleep(500);
    initPort(&hComTx, COMPORT_Tx, nComRate, nComBits, timeout);
    Sleep(500);

    while (true) {

 
        LPVOID msgOut[]={"Hi there person"};
    outputToPort(&hComTx, msgOut, 15+1);
    Sleep(500);


    char msgIn[BUFSIZE];
    inputFromPort(&hComRx, msgIn, BUFSIZE);
    printf("\nMessage Received: %s\n\n", msgIn);
    Sleep(500);
    }

     purgePort(&hComRx);
    purgePort(&hComTx);
    CloseHandle(hComRx);
    CloseHandle(hComTx);



    return 0;
}