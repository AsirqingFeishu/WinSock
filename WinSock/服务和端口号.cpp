#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")	// ���� WinSock �����
using namespace std;

int main(int argc, char** argv)
{
	/* ��ʼ��WinSock DLL*/
	WORD wVersionRequested = MAKEWORD(2, 2);	// ���ɰ汾��
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << " ����WinSock  DLL ʧ��" << endl;
		return 0;
	}
	struct servent* pServer;
	for (int i = 1; i < 10000; ++i)
	{
		pServer = getservbyport(htons(i), "TCP");
		if (pServer != nullptr)
		{
			cout << "��������" << pServer->s_name << endl;
			cout << "Э�飺" << pServer->s_proto << endl;
			cout << "�˿ںţ�" << ntohs(pServer->s_port) << endl;
		}
	}
	WSACleanup();
	return 0;

}