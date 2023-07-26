#include<iostream>
#include<WinSock2.h>		// 包含头文件

#pragma comment(lib,"ws2_32.lib")	// 连接Winsock 导入库

using namespace std;

int main(int argc,char** argv)
{
	/*  加载 WinSock  DLL  */
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);	// 生成版本号
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << " 加载WinSock  DLL  失败 " << endl;
		return 0;
	};

	u_short x, y = 0x1234;
	x = htons(y);		// 转为网络字节序
	cout << " 主机字节序 " << hex << y << endl;
	cout << "  网络字节序 " << hex << x << endl;

	u_long a, b = 0x1122AABB;
	a = htonl(b);
	cout << " 主机字节序 " << hex << b << endl;
	cout << "  网络字节序 " << hex << a << endl;

	return 0;
}

