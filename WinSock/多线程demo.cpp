#include<iostream>
#include<Windows.h>
#include<atlstr.h>
using namespace std;


// �����̺߳������̺߳����޲���
void ThreadFun1()
{
	for (int i = 1; i < 100; ++i)
	{
		Sleep(1000);							// ����1000ms
		cout << "i" << ", This is Thread 1" << endl;
	}
}
// �̺߳���
int ThreadFunc2(LPVOID lpParam)
{
	char* p = (char*)lpParam;
	Sleep(1000);
	cout << "This is Thread1,the string main thread given me is:" << *p << endl;
	return 0;
}
HANDLE hThread;				// �߳̾��
DWORD	ThreadID1;			// �߳�ID��
int main(int argc, char** argv)
{
	// �����߳�		�߳����ԡ���ջ��С���̺߳���������������������־���߳�ID
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun1, NULL, 0, &ThreadID1);
	for (int j = 0; j < 10; ++j)
	{
		Sleep(1000);
		cout << j << ", This is MainThread!\n";
	}

	system("pause");
	return 0;
}