
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rs232.h"

#define EX_FATAL 1

void initPort(HANDLE* hCom, wchar_t* COMPORT, int nComRate, int nComBits, COMMTIMEOUTS timeout)
{
    createPortFile(hCom, COMPORT);
    purgePort(hCom);
    SetComParms(hCom, nComRate, nComBits, timeout);
    purgePort(hCom);
}


void purgePort(HANDLE* hCom)
{
    PurgeComm(*hCom, PURGE_RXABORT | PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR);
}
void outputToPort(HANDLE* hCom, LPCVOID buf, DWORD szBuf)
{
    int i=0;
    DWORD NumberofBytesTransmitted;
    LPDWORD lpErrors = 0;
    LPCOMSTAT lpStat=0;

    i = WriteFile(
        *hCom,
        buf,
        szBuf,
        &NumberofBytesTransmitted,
        NULL
    );
    if (i == 0)
    {

        printf("\nRead Error : 0x%x\n", GetLastError());
        ClearCommError(hCom, lpErrors, lpStat);
    }
    else
        printf("\nSuccessful reception!, There were %ld bytes read\n", NumberofBytesTransmitted);

}
void inputFromPort(HANDLE* hCom, LPVOID buf, DWORD szBuf)
{
   int i=0;

   DWORD NumberofBytesRead;
   LPDWORD lpErrors=0;
   LPCOMSTAT lpStat=0;

   i = ReadFile(
   *hCom,
   buf,
   szBuf,
   &NumberofBytesRead,
   NULL
   );

   if (i==0)
   {

    printf("\nRead Error : 0x%x\n", GetLastError());
    ClearCommError(hCom, lpErrors, lpStat);
   } else 
        printf("\nSuccessful reception!, There were %ld bytes read\n", NumberofBytesRead);

}

// SUb functions
void createPortFile(HANDLE* hCom, wchar_t* COMPORT)
{
    *hCom = CreateFile(
        COMPORT,
        GENERIC_READ | GENERIC_WRITE,
        NULL,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
        );

    if (*hCom == INVALID_HANDLE_VALUE) {
        printf("\nFatal Error 0x%x: Unable to open\n", GetLastError());
    }
    else {
        printf("\nCOM is now open\n");
    }
}
static int SetComParms(HANDLE* hCom, int nComRate, int nComBits, COMMTIMEOUTS timeout)
{
    DCB dcb;

    memset(&dcb, 0, sizeof(dcb));
    dcb.DCBlength=sizeof(dcb);
    if (!GetCommState(*hCom, &dcb))
        return(0);


    dcb.BaudRate=nComRate;
    dcb.ByteSize=(BYTE)nComBits;
    dcb.Parity=0;
    dcb.StopBits=ONESTOPBIT;
    if(!SetCommState(*hCom, &dcb)) return 0;

    memset((void *) &timeout, 0, sizeof(timeout));
    timeout.ReadIntervalTimeout=500;
    timeout.ReadTotalTimeoutMultiplier=1;
    timeout.ReadTotalTimeoutConstant=5000;
    SetCommTimeouts(*hCom,&timeout);
    return 1;
}