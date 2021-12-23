#pragma once
#pragma comment(lib, "Ws2_32.lib")
#pragma warning( disable : 4996)

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <cstdio>
#include <msclr/marshal.h>
using namespace System;
using namespace msclr::interop;

#define PORT 777
#define SERVERADDR "127.0.0.1"

namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		String^ mStr;

		MyForm(void)
		{
			InitializeComponent();
			serialPort1->Open();
			timer1->Start();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
			serialPort1->Close();
			timer1->Stop();
		}
	private: System::IO::Ports::SerialPort^ serialPort1;
	private: System::Windows::Forms::Timer^ timer1;


	private: System::Windows::Forms::Label^ label1;
	protected:

	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// serialPort1
			// 
			this->serialPort1->PortName = L"COM4";
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &MyForm::serialPort1_DataReceived);
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(216, 129);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(46, 17);
			this->label1->TabIndex = 2;
			this->label1->Text = L"label1";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(507, 253);
			this->Controls->Add(this->label1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		label1->Text = mStr;
	}
	private: System::Void serialPort1_DataReceived(System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs^ e) {
		mStr = serialPort1->ReadLine();
		marshal_context^ marshal = gcnew marshal_context();
		const char* native = marshal->marshal_as<const char*>(mStr);
		char buff[1024];
		strcpy(buff, native);
		delete marshal;
		char servBuf[1024];
		int comLen;
		WSADATA wsaData;
		if (WSAStartup(0x202, &wsaData))
		{
		}
		SOCKET my_sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (my_sock == INVALID_SOCKET)
		{
			WSACleanup();
		}
		HOSTENT* hst;
		sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(PORT);
		if (inet_addr(SERVERADDR))
			dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
		else
			if (hst = gethostbyname(SERVERADDR))
				dest_addr.sin_addr.s_addr = ((unsigned long**)hst->h_addr_list)[0][0];
			else
			{
				closesocket(my_sock);
				WSACleanup();
			}
			comLen = 0;
			sendto(my_sock, &buff[0], sizeof(buff) - 1, 0, (sockaddr*)&dest_addr, sizeof(dest_addr));
			sockaddr_in server_addr;
			int server_addr_size = sizeof(server_addr);
			int n = recvfrom(my_sock, &servBuf[0], sizeof(servBuf) - 1, 0, (sockaddr*)&server_addr, &server_addr_size);
			if (n == SOCKET_ERROR)
			{
				closesocket(my_sock);
				WSACleanup();
			}
			servBuf[n] = 0;
		closesocket(my_sock);
		WSACleanup();
	}
};
}
