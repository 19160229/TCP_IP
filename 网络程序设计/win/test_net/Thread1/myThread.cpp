#include "windows.h"
#include "process.h"
#include "stdio.h"
#include "stdlib.h"

unsigned int CALLBACK MyThread(void * p)
{
  for (int i=1;i<=10;i++) 
  {
	  printf("�߳�ID��%d   ��ӡ��%d \n",GetCurrentThreadId(),i);
	  Sleep(1000);
  }
  return 1;
}



void main(void)
{
   HANDLE hs[2]; 
	   hs[0]= (HANDLE)_beginthreadex(NULL,0,MyThread,NULL,0,NULL);
   if (hs[0]) 
   {
	   printf("�߳�1������.\n");
   }

   hs[1] = (HANDLE)_beginthreadex(NULL,0,MyThread,NULL,0,NULL);
   if (hs[1]) 
   {
	   printf("�߳�2������.\n");
   }


WaitForMultipleObjects(2,hs,TRUE,INFINITE);
 printf("�߳�1���߳�2�����н�����\n");
 CloseHandle(hs[0]);
 CloseHandle(hs[1]);
 CRITICAL_SECTION
}