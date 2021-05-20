
#include "MyForm.h"


using namespace std;
using namespace System;
using namespace System::Windows::Forms;

void main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	CPPCLRSerialPort::MyForm MyForm;
	Application::Run(% MyForm);

}