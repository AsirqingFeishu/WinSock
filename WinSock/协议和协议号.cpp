#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main(int argc, char** argv)
{
	/* ��ʼ�� WinSock  DLL  */
	WORD wVersionRequested = MAKEWORD(2, 2);		// ���ɰ汾��2.2
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << " ����WinSock DLLʧ��" << endl;
		return 0;
	}
	struct protoent* pProto;
	for(int i=1;i<=256;++i)
	{
		if ((pProto = getprotobynumber(i)) != nullptr)
		{
			cout << "Э������" << pProto->p_name << endl;
			cout << "Э��ţ�" << pProto->p_proto << endl;
		}
	}
	WSACleanup();

	return 0;
}