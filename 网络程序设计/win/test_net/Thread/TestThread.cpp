#include "windows.h"
#include "stdlib.h"
#include "stdio.h"
#include "process.h"


unsigned int  CALLBACK  MyThread(void * param)
{
	//DWORD n=(DWORD)param;

	for (int i=1;i<11;i++)
	{
		printf("�߳�:%d  ��ӡ��%d \n",GetCurrentThreadId(),i);
		Sleep(2000);
	}
	return 1;
}




void main(void)
{
 
 DWORD ThreadID1=0,ThreadID2=0;
 u_int ID1=0,ID2=0;
 HANDLE   hThread1 =(HANDLE) _beginthreadex(NULL,0,MyThread,NULL,0,&ID1);
 if (hThread1!=0 )
 {
	 printf("�߳�1 ID:%d handle:%d������\n",ID1,hThread1);
 }
 int n=2;
 HANDLE  hThread2 =(HANDLE) _beginthreadex(NULL,0,MyThread,(void*)n,0,&ID2);
 if (hThread2!=0 )
 {
	 printf("�߳�2 ID:%d ������\n",ID2);
 }

 WaitForSingleObject(hThread1,INFINITE);
 WaitForSingleObject(hThread2,INFINITE);
 //ResumeThread(hThread1);
// WaitForSingleObject(hThread1,INFINITE);
 printf("�̣߳�������������\n");
 CloseHandle(hThread1);
 CloseHandle(hThread2);
}//main