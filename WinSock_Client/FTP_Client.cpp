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
	/*   定义网络连接变量  */
	SOCKET sock_client;				// 通信sock
	struct sockaddr_in addr_server;	// 服务器地址
	int addr_len = sizeof(struct sockaddr);

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
	/*  创建套接字  */
	if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "通信套接字创建失败....." << endl;
		cout << "错误代码：" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return 0;
	}
	
	/*  连接服务器  */
	char addr[20];
	cout << "请输入服务器的ip地址：";
	cin >> addr;
	memset((void*)&addr_server, 0, addr_len);
	addr_server.sin_port = htons(PORT);
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.S_un.S_addr = inet_addr(addr);

	if (connect(sock_client, (struct sockaddr*)&addr_server, addr_len) != 0)
	{
		cout << "connect  连接失败...." << endl;
		cout << "错误代码：" << WSAGetLastError() << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	/*   定义文件信息结构  */
	struct fileMessage fileMsg;				// 文件信息结构
	char OK[3]="OK", fileBuffer[500];			// OK信息，接收缓存
	long int file_len;							// 文件大小
	char filename[500] = "G:\\my file\\接收文件夹";
	// 创建保存文件目录
	_mkdir(filename);
	strcat(filename,"\\");
	/*   接收文件名+大小  */
	if (recv(sock_client, (char*)&fileMsg, sizeof(fileMsg), 0) < 0)
	{
		cout << "未接受到文件名及文件长度..." << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	file_len = htonl(fileMsg.fileSize);
	strcat(filename, fileMsg.filename);
	cout << "收到文件：" << fileMsg.filename << "   大小：" << file_len << endl;
	cout << "准备接收...." << endl;
	/*       创建文件       */
	ofstream outFile(filename, ios::out | ios::binary);
	if (!outFile.is_open())
	{
		cout << "Cannot open ....文件打开失败..." << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	/*   发送确认信息   */
	send(sock_client, OK, sizeof(OK), 0);
	/*   开始接收   */
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