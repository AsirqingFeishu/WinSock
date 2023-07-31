#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#define PORT 57839
using namespace std;

/*  ************************
	����������������ͻ��˽������ӣ����ͺͽ��յ���Ϣ�󣬹ر�����
	ѭ��ִ��
*******************************/

int main(int argc, char** argv)
{
	/*  ������ر���  */
	SOCKET sock_server, newsock;		// �������sock��ͨ��sock
	struct sockaddr_in addr;			// ���� server ��ַ�ṹ�����
	struct sockaddr_in client_addr;		// ���� client ��ַ�ṹ�����
	char msgbuf[265];					// ������Ϣ������
	char msg[] = "Connect  succeed .....\n";	// ������Ϣ

	/* ��ʼ��WinSock DLL */
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);		// ���ɰ汾��2.2
	if (0 != WSAStartup(wVersionRequested, &wsaData))
	{
		cout << "����WinSock.dll ʧ��" << endl;
		system("pause");
		return 0;
	};
	/*****�����׽��� *********/
	if ((sock_server = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		cout << "�������׽��ִ���ʧ��...." << endl;
		WSACleanup();
		system("pause");
		return 0;
	}
	/* *****��д���������ص�ַ��Ϣ************/
	int addr_len = sizeof(struct sockaddr_in);
	memset((void*)&addr, addr_len, 0);				// ��ַ��ʼ��Ϊ0
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);		// �����κε�ַ

	/* ********* �׽��ְ󶨵�ַ ************/
	if (bind(sock_server, (struct sockaddr*)&addr, addr_len) != 0)
	{
		cout << "�׽��� �͵�ַ��ʧ�� ...." << endl;
		cout << "������룺" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}
	/*  **********��������***********/
	if (listen(sock_server, 0) != 0)
	{
		cout << " listen ����ʧ��...." << endl << "������룺" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 0;
	}
	else
	{
		cout << "listenning...." << endl;
		/*   ѭ�����������շ�����  */
		int size;
		while (true)
		{

			if ((newsock = accept(sock_server, (struct sockaddr*)&client_addr, &addr_len)) == INVALID_SOCKET)
			{
				cout << "accept ������������ʧ��...." << endl;
				cout << "������룺" << WSAGetLastError() << endl;
				break;
			}
			else
			{
				cout << "�ɹ�����һ����������" << endl;
				size = send(newsock, msg, sizeof(msg), 0);	// ʹ��ͨ���׽��ַ�������
				if (size == SOCKET_ERROR)
				{
					cout << "������Ϣʧ��...." << endl;
					cout << "������룺" << WSAGetLastError() << endl;
					closesocket(newsock);
					continue;
				}
				else if (size == 0)
				{
					cout << "�Է��Ѿ��ر����ӡ�����" << endl;
					closesocket(newsock);
					continue;
				}
				else
				{
					cout << "��Ϣ���ͳɹ�..." << endl;
					if ((size = recv(newsock, msgbuf, sizeof(msgbuf), 0)) < 0)
					{
						cout << "������Ϣʧ��...." << endl;
						closesocket(newsock);
						continue;
					}
					else if (size == 0)
					{
						cout << "�Է��Ѿ��ر�����" << endl;
						closesocket(newsock);
						continue;
					}
					else
					{
						cout << "���յ���Ϣ��" << msgbuf << endl;
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