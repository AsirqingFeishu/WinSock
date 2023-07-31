#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>
#include<fstream>
#include<direct.h>
#include"fileMessage.h"
#define PORT 57839

#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main(int argv, char** argc)
{
	/*   �����������ӱ���  */
	SOCKET sock_client;				// ͨ��sock
	struct sockaddr_in addr_server;	// ��������ַ
	int addr_len = sizeof(struct sockaddr);

	/**********  ��ʼ��WinSock**********/
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);		// ���ɰ汾��
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << "���� WinSock  DLL ʧ��...." << endl;
		cout << "������룺" << WSAGetLastError() << endl;
		system("pause");
		return 0;
	}
	/*  �����׽���  */
	if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "ͨ���׽��ִ���ʧ��....." << endl;
		cout << "������룺" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return 0;
	}
	
	/*  ���ӷ�����  */
	char addr[20];
	cout << "�������������ip��ַ��";
	cin >> addr;
	memset((void*)&addr_server, 0, addr_len);
	addr_server.sin_port = htons(PORT);
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.S_un.S_addr = inet_addr(addr);

	if (connect(sock_client, (struct sockaddr*)&addr_server, addr_len) != 0)
	{
		cout << "connect  ����ʧ��...." << endl;
		cout << "������룺" << WSAGetLastError() << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	/*   �����ļ���Ϣ�ṹ  */
	struct fileMessage fileMsg;				// �ļ���Ϣ�ṹ
	char OK[3]="OK", fileBuffer[500];			// OK��Ϣ�����ջ���
	long int file_len;							// �ļ���С
	char filename[500] = "G:\\my file\\�����ļ���";
	// ���������ļ�Ŀ¼
	_mkdir(filename);
	strcat(filename,"\\");
	/*   �����ļ���+��С  */
	if (recv(sock_client, (char*)&fileMsg, sizeof(fileMsg), 0) < 0)
	{
		cout << "δ���ܵ��ļ������ļ�����..." << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	file_len = htonl(fileMsg.fileSize);
	strcat(filename, fileMsg.filename);
	cout << "�յ��ļ���" << fileMsg.filename << "   ��С��" << file_len << endl;
	cout << "׼������...." << endl;
	/*       �����ļ�       */
	ofstream outFile(filename, ios::out | ios::binary);
	if (!outFile.is_open())
	{
		cout << "Cannot open ....�ļ���ʧ��..." << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	/*   ����ȷ����Ϣ   */
	send(sock_client, OK, sizeof(OK), 0);
	/*   ��ʼ����   */
	int size;
	do
	{
		size = recv(sock_client, fileBuffer, sizeof(fileBuffer), 0);
		outFile.write(fileBuffer, size);
		file_len -= size;
	} while (file_len > 0 && size != 0);

	cout << "Transfer finished...." << endl;
	closesocket(sock_client);
	WSACleanup();
	system("pause");
	return 0;
}