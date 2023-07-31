#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#define PORT 57839
using namespace std;

int main(int argc, char** argv)
{
	/****** ������ر���  ***********/
	SOCKET sock_client;						// �ͻ���Sock
	struct sockaddr_in server_add;			// ��������ַ
	int addr_len = sizeof(struct sockaddr_in);
	char msgbuffer[1000];					// ���ա����ͻ���

	/********  ��ʼ��WinSock  **************/
	WORD wVersionRequested = MAKEWORD(2, 2);		// ���ɰ汾��
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << "����WinSock.dll ʧ��" << endl;
		return 0;
	}
	cout << "����WinSock.dll �ɹ�...." << endl;
	/************  ����ͨ���׽��� ***********/
	if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "�ͻ���ͨ���׽��ִ���ʧ��...." << endl;
		cout << "������룺" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return 0;
	}
	cout << "�ͻ���ͨ���׽��ִ����ɹ�...." << endl;
	/*********  ��д��������ַ ********/
	char IP[20];
	cout << "�����������ip��ַ: " << endl;
	cin >> IP;
	memset((void*)&server_add, 0, addr_len);
	server_add.sin_family = AF_INET;
	server_add.sin_port = htons(PORT);
	server_add.sin_addr.S_un.S_addr = inet_addr(IP);

	/***************  �������� ************/

	if (0 != connect(sock_client, (struct sockaddr*)&server_add, addr_len))
	{
		cout << "����ʧ��..." << endl << "������룺" << WSAGetLastError() << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	/*********** ������Ϣ����ʾ*************/
	int size;
	if ((size = recv(sock_client, msgbuffer, sizeof(msgbuffer), 0)) < 0)
	{
		cout << "������Ϣʧ��...." << endl << "������룺" << WSAGetLastError() << endl;
		closesocket(sock_client);
		WSACleanup();

		return 0;
	}
	else if (size == 0)
	{
		cout << "�Է��Ѿ��ر�����...." << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	else
	{
		cout << "The message from Server:  " << msgbuffer << endl;
	}

	/***************  �������뷢�͸�������***********/
	cout << "�����뷢�͸�����������Ϣ��";
	cin >> msgbuffer;
	if ((size = send(sock_client, msgbuffer, sizeof(msgbuffer), 0)) < 0)
	{
		cout << "������Ϣʧ�ܡ�������" << endl << "������룺" << WSAGetLastError() << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	else if (size == 0)
	{
		cout << "�������Ѿ��ر�����..." << endl;

	}
	else
		cout << "���ͳɹ�������" << endl;


	closesocket(sock_client);
	WSACleanup();
	system("pause");
	return 0;
}