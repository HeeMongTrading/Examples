
#include <strsafe.h>
#include "Serial.h"
CSerial::CSerial()
    :m_bOpened(false), m_hIDComDev(NULL) {

}
CSerial::~CSerial() {
    Close();
}
bool
CSerial::Open(int nPort, int nBaud) {
    if (m_bOpened)
        return (true);
    wchar_t szPort[25];
    StringCbPrintfW(szPort, sizeof(szPort), L"\\\\.\\COM%d", nPort);
    m_hIDComDev = CreateFileW(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
    if (m_hIDComDev == NULL)
        return (false);
    //------------------------------------------------------------------
    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = 0;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = 5000;
    SetCommTimeouts(m_hIDComDev, &CommTimeOuts);

    //------------------------------------------------------------------
    memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
    memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));
    m_OverlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_OverlappedWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    //------------------------------------------------------------------
    DCB dcb;
    dcb.DCBlength = sizeof(DCB);
    GetCommState(m_hIDComDev, &dcb);
    dcb.BaudRate = nBaud;
    dcb.ByteSize = 8;
    //dcb.Parity = Parity;
    //dcb.StopBits = StopBits;
    //dcb.fBinary = true;
    //dcb.fDsrSensitivity = false;
    //dcb.fParity = fParity;
    //dcb.fOutX = false;
    //dcb.fInX = false;
    //dcb.fNull = false;
    //dcb.fAbortOnError = true;
    //dcb.fOutxCtsFlow = false;
    //dcb.fOutxDsrFlow = false;
    //dcb.fDtrControl = DTR_CONTROL_DISABLE;
    //dcb.fDsrSensitivity = false;
    //dcb.fRtsControl = RTS_CONTROL_DISABLE;
    //dcb.fOutxCtsFlow = false;
    //dcb.fOutxCtsFlow = false;

    //------------------------------------------------------------------

    if (SetCommState(m_hIDComDev, &dcb) == false ||
        SetupComm(m_hIDComDev, 4096, 4096) == false ||    //버퍼사이즈
        m_OverlappedRead.hEvent == NULL || m_OverlappedWrite.hEvent == NULL) {

        if (m_OverlappedRead.hEvent != NULL)
            CloseHandle(m_OverlappedRead.hEvent);
        if (m_OverlappedWrite.hEvent != NULL)
            CloseHandle(m_OverlappedWrite.hEvent);
        if (m_hIDComDev != NULL)
            CloseHandle(m_hIDComDev);
        return (false);
    }
    m_bOpened = true;
    return (true);
}

//------------------------------------------------------------------------------------------------------------------------------------
bool
CSerial::Close(void) {
    if (m_bOpened == false || m_hIDComDev == NULL)
        return (true);
    if (m_OverlappedRead.hEvent != NULL)
        CloseHandle(m_OverlappedRead.hEvent);
    if (m_OverlappedWrite.hEvent != NULL)
        CloseHandle(m_OverlappedWrite.hEvent);
    if (m_hIDComDev != NULL)
        CloseHandle(m_hIDComDev);
    m_bOpened = false;
    m_hIDComDev = NULL;
    return (true);
}
//------------------------------------------------------------------------------------------------------------------------------------
bool
CSerial::WriteCommByte(unsigned char ucByte) {
    BOOL bWriteStat;
    DWORD dwBytesWritten;
    bWriteStat = WriteFile(m_hIDComDev, (LPSTR)&ucByte, 1, &dwBytesWritten, &m_OverlappedWrite);
    if (!bWriteStat && (GetLastError() == ERROR_IO_PENDING)) {
        if (WaitForSingleObject(m_OverlappedWrite.hEvent, 1000))
            dwBytesWritten = 0;
        else {
            GetOverlappedResult(m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, FALSE);
            m_OverlappedWrite.Offset += dwBytesWritten;
        }
    }
    return (true);
}
//------------------------------------------------------------------------------------------------------------------------------------
size_t
CSerial::WriteData(char* buffer, size_t size) {
    if (m_bOpened == false || m_hIDComDev == NULL)
        return (0);
    DWORD dwBytesWritten = 0;
    unsigned i;
    for (i = 0; i < size; i++) {
        WriteCommByte(buffer[i]);
        dwBytesWritten++;
    }
    return (size_t)dwBytesWritten;
}

//------------------------------------------------------------------------------------------------------------------------------------
size_t
CSerial::ReadData(void* buffer, size_t limit) {
    if (m_bOpened == false || m_hIDComDev == NULL)
        return (0);
    BOOL bReadStatus;
    DWORD dwBytesRead, dwErrorFlags;
    COMSTAT ComStat;
    ClearCommError(m_hIDComDev, &dwErrorFlags, &ComStat);
    if (!ComStat.cbInQue)
        return (0);
    dwBytesRead = (DWORD)ComStat.cbInQue;
    if (limit < (int)dwBytesRead)
        dwBytesRead = (DWORD)limit;
    bReadStatus = ReadFile(m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead);
    if (!bReadStatus) {
        if (GetLastError() == ERROR_IO_PENDING) {
            WaitForSingleObject(m_OverlappedRead.hEvent, 100);
            return((int)dwBytesRead);
        }
        return (0);
    }
    printf("Read: %s", dwBytesRead);
    return (size_t)dwBytesRead;
}
//------------------------------------------------------------------------------------------------------------------------------------ 
