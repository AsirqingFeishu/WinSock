#include<iostream>
#include<WinSock2.h>		// ����ͷ�ļ�

#pragma comment(lib,"ws2_32.lib")	// ����Winsock �����

using namespace std;

int main(int argc,char** argv)
{
	/*  ���� WinSock  DLL  */
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);	// ���ɰ汾��
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << " ����WinSock  DLL  ʧ�� " << endl;
		return 0;
	};

	u_short x, y = 0x1234;
	x = htons(y);		// תΪ�����ֽ���
	cout << " �����ֽ��� " << hex << y << endl;
	cout << "  �����ֽ��� " << hex << x << endl;

	u_long a, b = 0x1122AABB;
	a = htonl(b);
	cout << " �����ֽ��� " << hex << b << endl;
	cout << "  �����ֽ��� " << hex << a << endl;

	return 0;
}

