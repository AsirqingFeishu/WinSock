#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>	// ����ͷ�ļ�
#pragma comment(lib,"ws2_32.lib")	// ���� WinSock �����
using namespace std;

int main(int agrc, char** agrv)
{
	struct hostent* hptr;
	char** pptr=nullptr;
	char hostname[256];		// ���ڴ�Ż�ȡ�������ƻ������Զ����������
	/*  ��ʼ��WinSock  DLL */

	WORD wVersionRequested = MAKEWORD(2, 2);			// �汾���
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << "  ���� WinSock  DLL  ʧ�� " << endl;
		return 0;
	}
	if (gethostname(hostname, sizeof(hostname)))			// ��ȡ��������
	{
		cout << "��ȡ������ʧ�ܣ�" << endl;
		WSACleanup();		
		return 0;
	}
	cout << " �������֣�" << hostname << endl;
	if ((hptr = gethostbyname(hostname)) == nullptr)		// ��ȡ����ip��ַ
	{
		cout << "ͨ����������ȡ����IP��ַʧ�ܣ�" << endl;
		WSACleanup();
		return 0;
	}
	/*  ���ip��ַi*/
	pptr = hptr->h_addr_list;
	cout << "����IP��ַ��" << endl;
	while (*pptr != nullptr)
	{
		cout << inet_ntoa(*(struct in_addr*)(*pptr)) << endl;
		++pptr;
	}

	/* �Ӽ���������������� */
	cout << "����Ҫ����������" << endl;
	cin >> hostname;
	if ((hptr = gethostbyname(hostname)) == nullptr)
	{
		cout << "���ֽ���ʧ�ܣ�" << endl;
		WSACleanup();
		return 0;
	}
	
	// ���������Ӧip
	pptr = hptr->h_addr_list;
	cout << "����ip��ַ" << endl;
	while (*pptr != nullptr)
	{
		cout << inet_ntoa(*(struct in_addr*)(*pptr)) << endl;
		++pptr;
	}

	WSACleanup();
	return 0;
}
