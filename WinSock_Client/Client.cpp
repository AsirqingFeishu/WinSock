#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#define PORT 57839
using namespace std;

int main(int argc, char** argv)
{
	/****** 定义相关变量  ***********/
	SOCKET sock_client;						// 客户端Sock
	struct sockaddr_in server_add;			// 服务器地址
	int addr_len = sizeof(struct sockaddr_in);
	char msgbuffer[1000];					// 接收、发送缓存

	/********  初始化WinSock  **************/
	WORD wVersionRequested = MAKEWORD(2, 2);		// 生成版本号
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << "加载WinSock.dll 失败" << endl;
		return 0;
	}
	cout << "加载WinSock.dll 成功...." << endl;
	/************  创建通信套接字 ***********/
	if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "客户端通信套接字创建失败...." << endl;
		cout << "错误代码：" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return 0;
	}
	cout << "客户端通信套接字创建成功...." << endl;
	/*********  填写服务器地址 ********/
	char IP[20];
	cout << "请输入服务器ip地址: " << endl;
	cin >> IP;
	memset((void*)&server_add, 0, addr_len);
	server_add.sin_family = AF_INET;
	server_add.sin_port = htons(PORT);
	server_add.sin_addr.S_un.S_addr = inet_addr(IP);

	/***************  建立连接 ************/

	if (0 != connect(sock_client, (struct sockaddr*)&server_add, addr_len))
	{
		cout << "连接失败..." << endl << "错误代码：" << WSAGetLastError() << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	/*********** 接收信息并显示*************/
	int size;
	if ((size = recv(sock_client, msgbuffer, sizeof(msgbuffer), 0)) < 0)
	{
		cout << "接收信息失败...." << endl << "错误代码：" << WSAGetLastError() << endl;
		closesocket(sock_client);
		WSACleanup();

		return 0;
	}
	else if (size == 0)
	{
		cout << "对方已经关闭连接...." << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	else
	{
		cout << "The message from Server:  " << msgbuffer << endl;
	}

	/***************  键盘输入发送给服务器***********/
	cout << "请输入发送给服务器的信息：";
	cin >> msgbuffer;
	if ((size = send(sock_client, msgbuffer, sizeof(msgbuffer), 0)) < 0)
	{
		cout << "发送信息失败。。。。" << endl << "错误代码：" << WSAGetLastError() << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 0;
	}
	else if (size == 0)
	{
		cout << "服务器已经关闭连接..." << endl;

	}
	else
		cout << "发送成功！！！" << endl;


	closesocket(sock_client);
	WSACleanup();
	system("pause");
	return 0;
}