#pragma once
// Serial.h


#include <windows.h>
class CSerial
{
public:
    CSerial();
    ~CSerial();
    bool    Open(int nPort = 2, int nBaud = 38400);
    bool    Close(void);
    size_t    ReadData(void*, size_t);
    size_t    WriteData(char*, size_t);

    bool    IsOpened(void) { return(m_bOpened); }

    bool    WriteCommByte(unsigned char);
protected:
    HANDLE    m_hIDComDev;
    OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
    bool    m_bOpened;
};
