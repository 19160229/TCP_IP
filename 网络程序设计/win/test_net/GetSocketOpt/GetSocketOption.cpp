#include <winsock2.h>
#include <ws2tcpip.h>

#include "stdlib.h"
#include "stdio.h"

#define OPT_FLAG 0
#define OPT_INT 1
#define OPT_LINGER 2
#define OPT_TIMEVAL 3




   void print_sockopt(union _val*, int, int);
    union _val
    {
    DWORD i_val;
    DWORD l_val;
    char c_val[10];
    struct linger linger_val;
   } val;

    struct _sock_opts
    {
    char * opt_str;
    int opt_level;
    int opt_name;
    int opt_type;
    } sock_opts[]={
  "SO_BROADCAST",SOL_SOCKET,SO_BROADCAST, OPT_FLAG,
  "SO_DEBUG",  SOL_SOCKET,SO_DEBUG, OPT_FLAG,
  "SO_DONTROUTE",  SOL_SOCKET,SO_DONTROUTE,OPT_FLAG,
  "SO_ERROR", SOL_SOCKET,SO_ERROR, OPT_INT,
  "SO_KEEPALIVE",SOL_SOCKET,SO_KEEPALIVE, OPT_FLAG,
  "SO_LINGER",SOL_SOCKET,SO_LINGER,OPT_LINGER,
  "SO_OOBINLINE", SOL_SOCKET,SO_OOBINLINE, OPT_FLAG,
  "SO_RCVBUF",SOL_SOCKET,SO_RCVBUF,OPT_INT,
  "SO_SNDBUF",SOL_SOCKET,SO_SNDBUF,OPT_INT,
 // "SO_RCVLOWAT",SOL_SOCKET,SO_RCVLOWAT,OPT_INT,
 // "SO_SNDLOWAT",SOL_SOCKET,SO_SNDLOWAT,OPT_INT,
  "SO_MAX_MSG_SIZE(仅对UDP)",SOL_SOCKET,SO_MAX_MSG_SIZE,OPT_INT,
  "SO_RCVTIMEO",SOL_SOCKET,SO_RCVTIMEO,OPT_INT,
  "SO_SNDTIMEO",SOL_SOCKET,SO_SNDTIMEO,OPT_INT,
  "SO_REUSEADDR",SOL_SOCKET,SO_REUSEADDR, OPT_FLAG,
  "SO_TYPE",SOL_SOCKET,SO_TYPE,OPT_INT,  
  "IP_DONTFRAGMENT",IPPROTO_IP,IP_DONTFRAGMENT,OPT_FLAG,
//  "IP_TOS",IPPROTO_IP,IP_TOS,OPT_INT,
  "IP_TTL",IPPROTO_IP,IP_TTL, OPT_INT,
 // "IP_RECEIVE_BROADCAST",IPPROTO_IP,IP_RECEIVE_BROADCAST,OPT_FLAG,
//  "TCP_MAXSEG", IPPROTO_TCP,TCP_MAXSEG,OPT_INT,
  "TCP_NODELAY",IPPROTO_TCP,TCP_NODELAY,OPT_FLAG,
  NULL,0,0,NULL
  };
  int main()
  {
    WSADATA wsaData;
    if (  WSAStartup(0x0202,&wsaData)!=0 ) 
	{
		printf("WSAStartup出错!出错号：%d \n",GetLastError());
		return -1;
    } 

    SOCKET sockfd,sockfd1;
	int len;
    struct _sock_opts* ptr;

   // sockfd = socket(AF_INET,SOCK_STREAM,0);
	sockfd = socket(AF_INET,SOCK_DGRAM,0);


    for(ptr=sock_opts;ptr->opt_str!=NULL;ptr++)
    {
          printf("%s:",ptr->opt_str);
           len = sizeof(val);
           if (getsockopt(sockfd, ptr->opt_level, ptr ->opt_name,(char*)&val, &len) == SOCKET_ERROR) 
              {
			   //if (getsockopt(sockfd1, ptr->opt_level, ptr ->opt_name,(char*)&val, &len) == SOCKET_ERROR) 
			   //{
				   printf("getsockopt 出错！错误代码：%d.\n",GetLastError());
                   continue;
		   //}//if
		   }//if
			  
               print_sockopt(&val,len, ptr -> opt_type);
              
   }/*for*/
	closesocket(sockfd);
//	closesocket(sockfd1);
	WSACleanup();
	return 0;
 }
 void print_sockopt(union _val * pval, int len, int type)
 {  
   switch (type) 
    {
         case OPT_FLAG:
           if (len!= sizeof(BOOL))
                 fprintf(stderr, "size ( %d) not sizeof(int) \n", len);
           else
               printf(" %s \n", (pval->i_val == 0) ? "off" : "on");
           break;
         case OPT_INT:
           if (len != sizeof(DWORD))
               fprintf(stderr, "size ( %d) not sizeof( int)\n", len);
           else
              printf(" %ld \n", pval->i_val) ;
           break;
         case OPT_LINGER:
           if (len != sizeof(struct linger))
                fprintf(stderr, "size ( %d) not sizeof(struct linger)\n", len);
           else
                printf("l_onoff = %d, l_linger = %d\n", pval->linger_val.l_onoff,pval -> linger_val.l_linger);
              break;
         case OPT_TIMEVAL:
            if (len != sizeof(DWORD))
              fprintf(stderr, "size (%d) not sizeof(struct timeval)\n", len);
            else
              printf(" %ld 毫秒\n", pval->i_val);
              break;
        default:
           fprintf( stderr, "unknown option type. \n");
    }/*switch*/
}
