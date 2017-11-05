// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "server.h"
#include "afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

void toUppercase(char* inputString){
	for (int index = 0; index < strlen(inputString); index++){
		if (inputString[index] >= 'a' && inputString[index] <= 'z'){
			//ký tự là chữ cái thường.
			inputString[index] = inputString[index] - 32;
		}
		else {
			//ký tự là ký tự hoa hoặc là ký tự đặc biệt.
			inputString[index] = inputString[index];
		}
	}
}
bool compare(char name1[25], char name2[25]){
	if (strlen(name1) != strlen(name2))
		return false;
	else{
		toUppercase(name1);
		toUppercase(name2);
		for (int j = 0; j < strlen(name2); j++)
			if (name1[j] != name2[j])
			return false;
		}
	return true;
}
bool compareChar(char nameClient[10][25], int nClient, char name[25])
{
	int size = strlen(name);
	for (int i = 0; i < nClient; i++)
		if (compare(nameClient[i], name))
			return true;
	return false;
}

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
			int nClient;
			CSocket server;
			if (AfxSocketInit() == FALSE)
			{
				cout << "Khong the khoi tao Socket Library";
				return FALSE;
			}
			// Tao socket cho server, dang ky port la 1234, giao thuc TCP
			if (server.Create(1234, SOCK_STREAM, NULL) == 0)
			{
				cout << "Khoi tao that bai !!!" << endl;
				cout << server.GetLastError();
				return FALSE;
			}
			else{
				cout << "Server khoi tao thanh cong !!!" << endl;
				do{
					cout << "Nhap so nguoi choi: ";
					cin >> nClient;
					if (nClient <= 0)
						cout << "Moi ban nhap lai";
				} while (nClient <= 0);

				if (server.Listen(nClient) == FALSE){// lang nghe n client
					cout << "Khong the lang nghe tren port nay !!!" << endl;
					server.Close();
					return FALSE;
				}
			}
			CSocket client[10];
			char nameClient[10][25];

			//tao 1 socket de duy tri viec ket noi va trao doi du lieu
			for (int i = 0; i < nClient; i++)
			{
				char* temp;
				int nameSize;
				
				if (server.Accept(client[i])){
					cout << "Da co Client " << i + 1 << " ket noi!!!" << endl << endl;
				}
				
				//nhận tên từ client
				do{
					client[i].Receive((char*)&nameSize, sizeof(int), 0);
					temp = new char[nameSize + 1];
					client[i].Receive((char*)temp, nameSize, 0);

					temp[nameSize] = '\0';
					if (compareChar(*&nameClient, i, temp) == true){
						//thong bao cho client ten da trung lap nhap lai
						int check = 1;
						client[i].Send((char*)&check, sizeof(int), 0);
					}
					else{
						int check = 0;
						client[i].Send((char*)&check, sizeof(int), 0);
					}
				} while (compareChar(*&nameClient, i, temp) == true);
				for (int j = 0; j <= nameSize; j++){
					nameClient[i][j] = temp[j];
				}
				delete temp;
			}
			//thông báo tất cả người chơi khi đủ
			int nameSize;
			for (int i = 0; i < nClient; i++)
			{
				client[i].Send((char*)&nClient, sizeof(int), 0);
				for (int j = 0; j < nClient; j++){
					nameSize = strlen(nameClient[i]);
					client[i].Send(&nameSize, sizeof(nameSize), 0);
					client[i].Send(nameClient[i], nameSize, 0);
				}
			}
			

			for (int i = 0; i < nClient; i++)
			{

			}
			//
			//fflush(stdin);
			//	do{
			//		char serverMsg[100];
			//		int msgSize;
			//		char *temp = NULL;
			//		cout << "Server say : ";

			//		cin.getline(serverMsg, 100);

			//		msgSize = strlen(serverMsg);
			//		for (int i = 0; i < nClient; i++)
			//		{
			//			//gui do dai thong diep de server biet duoc do dai nay
			//			client[i].Send(&msgSize, sizeof(msgSize), 0);
			//			//gui thong diep voi do dai la MsgSize
			//			client[i].Send(serverMsg, msgSize, 0);
			//		}
			//		for (int i = 0; i < nClient; i++)
			//		{
			//			int MsgSize;
			//			char *temp;

			//			//Nhan thong diep tu Client
			//			client[i].Receive((char*)&MsgSize, sizeof(int), 0);
			//			temp = new char[MsgSize + 1];
			//			client[i].Receive((char*)temp, MsgSize, 0);

			//			temp[MsgSize] = '\0';
			//			cout << "Client " << i << " say: " << temp << endl;
			//			if (client[i] == NULL)
			//			{
			//				client[i].ShutDown(2);
			//				break;
			//			}
			//			delete temp;
			//		}
			//		
			//	} while (true);
			//	client[0].Close();
			//	server.Close();
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
