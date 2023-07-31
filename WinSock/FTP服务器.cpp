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
	/********** 定义sock变量 **********/
	SOCKET sock_server;							// 监听sock
	SOCKET newsock;								// 通信sock
	struct sockaddr_in addr, client_addr;			// 地址
	int addr_len = sizeof(struct sockaddr_in);		// 地址长度

	/**********  初始化WinSock**********/
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);		// 生成版本号
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << "加载 WinSock  DLL 失败...." << endl;
		cout << "错误代码：" << WSAGetLastError() << endl;
		system("pause");
		return 0;
	}
	/************** 创建监听套接字 *************/
	if ((sock_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "监听套接字创建失败....." << endl;
		cout << "错误代码：" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return 0;
	}
	/***********   初始化ip地址*************/
	memset((void*)&addr, 0, addr_len);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;
	/**********  绑定套接字和ip地址**********/
	if (bind(sock_server, (struct sockaddr*)&addr, addr_len) != 0)
	{
		cout << "绑定失败....." << endl;
		cout << "错误代码：" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}
	/***************** 开始监听*************/
	if (listen(sock_server, 5) != 0)
	{
		cout << "listen监听失败....." << endl;
		cout << "错误代码：" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}
	/************  输入传输文件**********/
	cout << "请输入要传输得文件路径：";
	char fileName[500];
	cin.getline(fileName, 500);

	cout << "等待连接....." << endl;
	/********  开始连接*********/
	if ((newsock = accept(sock_server, (struct sockaddr*)&client_addr, &addr_len)) == INVALID_SOCKET)
	{
		cout << "accept连接失败....." << endl;
		cout << "错误代码：" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}
	cout << "connect from : " << inet_ntoa(client_addr.sin_addr) << "....." << endl;
	/***********  发送文件名+文件大小*******/
	struct fileMessage fileMsg;				// 文件信息结构体
	char OK[3], fileBuffer[1000];			// ok确认信息，发送缓存

	// 处理文件名
	int size = strlen(fileName);
	while (fileName[size] != '\\' && size > 0)
	{
		--size;
	}
	strcpy(fileMsg.filename, fileName + size + 1);
	//处理文件大小
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
	// 发送
	send(newsock, (char*)&fileMsg, sizeof(struct fileMessage),0);

	/****** 等待接收对方 OK信息***********/
	if (recv(newsock, OK, sizeof(OK), 0) <= 0)
	{
		cout << "接收OK失败，程序退出......" << endl;
		cout << "错误代码：" << WSAGetLastError() << endl;
		closesocket(newsock);
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}

	/*******  确定ok，发送文件**********/
	if (strcmp(OK, "OK") == 0)
	{
		while (!inFile.eof())
		{
			inFile.read(fileBuffer, sizeof(fileBuffer));
			size = inFile.gcount();							// 实际读取字节数
			send(newsock, fileBuffer, size, 0);
		}
		cout << "file transfer finished...." << endl;
		inFile.close();
	}
	else
	{
		cout << "对方无法接收文件!" << endl;

	}
	closesocket(newsock);
	closesocket(sock_server);
	WSACleanup();
	return 0;
}