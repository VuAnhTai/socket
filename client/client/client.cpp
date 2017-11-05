// client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "client.h"
#include "afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			CSocket client;
			if (AfxSocketInit() == FALSE){
				cout << "Khong the khoi tao Socket Library";
				return FALSE;
			}

			client.Create();

			// Ket noi den server
			if (client.Connect(_T("127.0.0.1"), 1234) != 0){
				cout << "Ket noi thanh cong !!!" << endl << endl;
				//Nhan tu server , cho biet day la client thu may
				int id, check;
				//đặt tên cho client
				do{
					char nameClient[25]; int sizeName;
					fflush(stdin);
					cout << "Nhap Ten Client: ";
					cin.getline(nameClient, 25);
					sizeName = strlen(nameClient);
					client.Send(&sizeName, sizeof(sizeName), 0);
					client.Send(nameClient, sizeName, 0);
					//nhận thông điệp kiểm tra có bị trùng tên không
					client.Receive((char*)&check, sizeof(check), 0);
					if (check == 1){
						cout << "Moi ban nhap lai ten moi: ";
					}
				}while(check == 1);

				//Bắt đầu thao tác:
				int nClient, nameSize;
				client.Receive((char*)&nClient, sizeof(nClient), 0);
				cout << "Danh sach nguoi choi: " << endl;
				for (int i = 0; i < nClient; i++)
				{
					client.Receive((char*)&nameSize, sizeof(int), 0);
					char* temp = new char[nameSize + 1];
					client.Receive((char*)temp, nameSize, 0);
					temp[nameSize] = '\0';
					cout << i << ".		" << temp << endl;
				}
				//char clientMsg[100];
				//int msgSize;
				//char* temp;
				//string *s = NULL;
				//do
				//{
				//	
				//	
				//	//nhan thong diep tu server
				//	client.Receive((char*)&msgSize, sizeof(int), 0);
				//	temp = new char[msgSize + 1];
				//	client.Receive((char*)temp, msgSize, 0);
				//	temp[msgSize] = '\0';
				//	cout << "Server say: " << temp << endl;
				//	cout << "Client say: ";
				//	cin.getline(clientMsg, 100);

				//	msgSize = strlen(clientMsg);

				//	client.Send(&msgSize, sizeof(msgSize), 0);

				//	client.Send(clientMsg, msgSize, 0);
				//	delete temp;
				//} while (true);
				//client.Close();
			}

		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
