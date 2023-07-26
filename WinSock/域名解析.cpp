#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>	// 包含头文件
#pragma comment(lib,"ws2_32.lib")	// 链接 WinSock 导入库
using namespace std;

int main(int agrc, char** agrv)
{
	struct hostent* hptr;
	char** pptr=nullptr;
	char hostname[256];		// 用于存放获取本机名称或输入的远程主机域名
	/*  初始化WinSock  DLL */

	WORD wVersionRequested = MAKEWORD(2, 2);			// 版本序号
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		cout << "  加载 WinSock  DLL  失败 " << endl;
		return 0;
	}
	if (gethostname(hostname, sizeof(hostname)))			// 获取本地名字
	{
		cout << "获取主机名失败！" << endl;
		WSACleanup();		
		return 0;
	}
	cout << " 本地名字：" << hostname << endl;
	if ((hptr = gethostbyname(hostname)) == nullptr)		// 获取本地ip地址
	{
		cout << "通过主机名获取本机IP地址失败！" << endl;
		WSACleanup();
		return 0;
	}
	/*  输出ip地址i*/
	pptr = hptr->h_addr_list;
	cout << "本机IP地址：" << endl;
	while (*pptr != nullptr)
	{
		cout << inet_ntoa(*(struct in_addr*)(*pptr)) << endl;
		++pptr;
	}

	/* 从键盘输入解析的域名 */
	cout << "输入要解析的域名" << endl;
	cin >> hostname;
	if ((hptr = gethostbyname(hostname)) == nullptr)
	{
		cout << "名字解析失败！" << endl;
		WSACleanup();
		return 0;
	}
	
	// 输出域名对应ip
	pptr = hptr->h_addr_list;
	cout << "主机ip地址" << endl;
	while (*pptr != nullptr)
	{
		cout << inet_ntoa(*(struct in_addr*)(*pptr)) << endl;
		++pptr;
	}

	WSACleanup();
	return 0;
}
