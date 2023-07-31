#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>
#include<fstream>
#include "fileMessge.h"

#define PORT 57839
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main(int argc, char** argv)
{
	/********** ����sock���� **********/
	SOCKET sock_server;							// ����sock
	SOCKET newsock;								// ͨ��sock
	struct sockaddr_in addr, client_addr;			// ��ַ
	int addr_len = sizeof(struct sockaddr_in);		// ��ַ����

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
	/************** ���������׽��� *************/
	if ((sock_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "�����׽��ִ���ʧ��....." << endl;
		cout << "������룺" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return 0;
	}
	/***********   ��ʼ��ip��ַ*************/
	memset((void*)&addr, 0, addr_len);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;
	/**********  ���׽��ֺ�ip��ַ**********/
	if (bind(sock_server, (struct sockaddr*)&addr, addr_len) != 0)
	{
		cout << "��ʧ��....." << endl;
		cout << "������룺" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}
	/***************** ��ʼ����*************/
	if (listen(sock_server, 5) != 0)
	{
		cout << "listen����ʧ��....." << endl;
		cout << "������룺" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}
	/************  ���봫���ļ�**********/
	cout << "������Ҫ������ļ�·����";
	char fileName[500];
	cin.getline(fileName, 500);

	cout << "�ȴ�����....." << endl;
	/********  ��ʼ����*********/
	if ((newsock = accept(sock_server, (struct sockaddr*)&client_addr, &addr_len)) == INVALID_SOCKET)
	{
		cout << "accept����ʧ��....." << endl;
		cout << "������룺" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}
	cout << "connect from : " << inet_ntoa(client_addr.sin_addr) << "....." << endl;
	/***********  �����ļ���+�ļ���С*******/
	struct fileMessage fileMsg;				// �ļ���Ϣ�ṹ��
	char OK[3], fileBuffer[1000];			// okȷ����Ϣ�����ͻ���

	// �����ļ���
	int size = strlen(fileName);
	while (fileName[size] != '\\' && size > 0)
	{
		--size;
	}
	strcpy(fileMsg.filename, fileName + size + 1);
	//�����ļ���С
	ifstream inFile(fileName, ios::in | ios::binary);
	if (!inFile.is_open())
	{
		cout << "Cannot open: " << fileMsg.filename << endl;
		closesocket(newsock);
		closesocket(sock_server);
		WSACleanup();
		return 0;
	}
	inFile.seekg(0, ios::end);
	size = inFile.tellg();
	inFile.seekg(0, ios::beg);
	fileMsg.fileSize = htonl(size);
	// ����
	send(newsock, (char*)&fileMsg, sizeof(struct fileMessage),0);

	/****** �ȴ����նԷ� OK��Ϣ***********/
	if (recv(newsock, OK, sizeof(OK), 0) <= 0)
	{
		cout << "����OKʧ�ܣ������˳�......" << endl;
		cout << "������룺" << WSAGetLastError() << endl;
		closesocket(newsock);
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}

	/*******  ȷ��ok�������ļ�**********/
	if (strcmp(OK, "OK") == 0)
	{
		while (!inFile.eof())
		{
			inFile.read(fileBuffer, sizeof(fileBuffer));
			size = inFile.gcount();							// ʵ�ʶ�ȡ�ֽ���
			send(newsock, fileBuffer, size, 0);
		}
		cout << "file transfer finished...." << endl;
		inFile.close();
	}
	else
	{
		cout << "�Է��޷������ļ�!" << endl;

	}
	closesocket(newsock);
	closesocket(sock_server);
	WSACleanup();
	return 0;
}