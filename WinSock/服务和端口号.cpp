#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")	// 链接 WinSock 导入库
using namespace std;

int main(int argc, char** argv)
{
	/* 初始化WinSock DLL*/
	WORD wVersionRequested = MAKEWORD(2, 2);	// 生成版本号
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << " 加载WinSock  DLL 失败" << endl;
		return 0;
	}
	struct servent* pServer;
	for (int i = 1; i < 10000; ++i)
	{
		pServer = getservbyport(htons(i), "TCP");
		if (pServer != nullptr)
		{
			cout << "服务名：" << pServer->s_name << endl;
			cout << "协议：" << pServer->s_proto << endl;
			cout << "端口号：" << ntohs(pServer->s_port) << endl;
		}
	}
	WSACleanup();
	return 0;

}