// SerialTest.cpp : Defines the entry point for the console application.
//

#include "Serial.h"
int main()
{
    CSerial            m_cSerial;
    char data[1024] = { 0, };
    memset(data, 0, sizeof(data));
    m_cSerial.Open(4, 9600);

    while(1){
    //읽을때
    m_cSerial.ReadData(data, 7);
    //쓸때
    data[0] = 'S';
    data[1] = 'T';
    data[2] = 0;
    m_cSerial.WriteData(data, 3);
    
    Sleep(100);
    }
    return 0;
}