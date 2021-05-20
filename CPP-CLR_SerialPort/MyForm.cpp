#include "MyForm.h"

inline System::Void CPPCLRSerialPort::MyForm::Button1_Click(System::Object^ sender, System::EventArgs^ e) {
	serialPort1->Open();
}

inline System::Void CPPCLRSerialPort::MyForm::Button2_Click(System::Object^ sender, System::EventArgs^ e) {
	serialPort1->Write("1");
}

inline System::Void CPPCLRSerialPort::MyForm::Button3_Click(System::Object^ sender, System::EventArgs^ e) {
	serialPort1->Write("2");
}
