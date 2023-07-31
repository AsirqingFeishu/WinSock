#include<iostream>
#include<Windows.h>
#include<atlstr.h>
using namespace std;


// 定义线程函数，线程函数无参数
void ThreadFun1()
{
	for (int i = 1; i < 100; ++i)
	{
		Sleep(1000);							// 阻塞1000ms
		cout << "i" << ", This is Thread 1" << endl;
	}
}
// 线程函数
int ThreadFunc2(LPVOID lpParam)
{
	char* p = (char*)lpParam;
	Sleep(1000);
	cout << "This is Thread1,the string main thread given me is:" << *p << endl;
	return 0;
}
HANDLE hThread;				// 线程句柄
DWORD	ThreadID1;			// 线程ID　
int main(int argc, char** argv)
{
	// 创建线程		线程属性、堆栈大小、线程函数、函数参数、附近标志、线程ID
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun1, NULL, 0, &ThreadID1);
	for (int j = 0; j < 10; ++j)
	{
		Sleep(1000);
		cout << j << ", This is MainThread!\n";
	}

	system("pause");
	return 0;
}