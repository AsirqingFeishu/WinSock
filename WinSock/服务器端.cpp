#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#define PORT 57839
using namespace std;

/*  ************************
	服务器程序依次与客户端进行连接，发送和接收到信息后，关闭连接
	循环执行
*******************************/

int main(int argc, char** argv)
{
	/*  定义相关变量  */
	SOCKET sock_server, newsock;		// 定义监听sock和通信sock
	struct sockaddr_in addr;			// 保存 server 地址结构体变量
	struct sockaddr_in client_addr;		// 保存 client 地址结构体变量
	char msgbuf[265];					// 接收消息缓冲区
	char msg[] = "Connect  succeed .....\n";	// 发送信息

	/* 初始化WinSock DLL */
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);		// 生成版本号2.2
	if (0 != WSAStartup(wVersionRequested, &wsaData))
	{
		cout << "加载WinSock.dll 失败" << endl;
		system("pause");
		return 0;
	};
	/*****创建套接字 *********/
	if ((sock_server = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		cout << "服务器套接字创建失败...." << endl;
		WSACleanup();
		system("pause");
		return 0;
	}
	/* *****填写服务器本地地址信息************/
	int addr_len = sizeof(struct sockaddr_in);
	memset((void*)&addr, addr_len, 0);				// 地址初始化为0
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);		// 监听任何地址

	/* ********* 套接字绑定地址 ************/
	if (bind(sock_server, (struct sockaddr*)&addr, addr_len) != 0)
	{
		cout << "套接字 和地址绑定失败 ...." << endl;
		cout << "错误代码：" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}
	/*  **********启动监听***********/
	if (listen(sock_server, 0) != 0)
	{
		cout << " listen 监听失败...." << endl << "错误代码：" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}
	else
	{
		cout << "listenning...." << endl;
		/*   循环接收请求并收发数据  */
		int size;
		while (true)
		{

			if ((newsock = accept(sock_server, (struct sockaddr*)&client_addr, &addr_len)) == INVALID_SOCKET)
			{
				cout << "accept 接收连接请求失败...." << endl;
				cout << "错误代码：" << WSAGetLastError() << endl;
				break;
			}
			else
			{
				cout << "成功接收一个连接请求！" << endl;
				size = send(newsock, msg, sizeof(msg), 0);	// 使用通信套接字发送数据
				if (size == SOCKET_ERROR)
				{
					cout << "发送消息失败...." << endl;
					cout << "错误代码：" << WSAGetLastError() << endl;
					closesocket(newsock);
					continue;
				}
				else if (size == 0)
				{
					cout << "对方已经关闭连接。。。" << endl;
					closesocket(newsock);
					continue;
				}
				else
				{
					cout << "消息发送成功..." << endl;
					if ((size = recv(newsock, msgbuf, sizeof(msgbuf), 0)) < 0)
					{
						cout << "接收信息失败...." << endl;
						closesocket(newsock);
						continue;
					}
					else if (size == 0)
					{
						cout << "对方已经关闭连接" << endl;
						closesocket(newsock);
						continue;
					}
					else
					{
						cout << "接收到消息：" << msgbuf << endl;
						closesocket(newsock);
					
					}
				}
			}
		}
	}

	closesocket(sock_server);
	WSACleanup();
	system("pause");
	return 0;
}