#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main(int argc, char** argv)
{
	/* 初始化 WinSock  DLL  */
	WORD wVersionRequested = MAKEWORD(2, 2);		// 生成版本号2.2
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << " 加载WinSock DLL失败" << endl;
		return 0;
	}
	struct protoent* pProto;
	for(int i=1;i<=256;++i)
	{
		if ((pProto = getprotobynumber(i)) != nullptr)
		{
			cout << "协议名：" << pProto->p_name << endl;
			cout << "协议号：" << pProto->p_proto << endl;
		}
	}
	WSACleanup();

	return 0;
}