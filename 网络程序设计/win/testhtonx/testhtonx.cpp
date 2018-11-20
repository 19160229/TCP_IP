#include "winsock2.h"
#include "stdio.h"

void main(void)
{
 u_long ul = 0xaabbccdd;
 u_long nul = 0;
 nul = htonl(ul);
 printf("host:%x  net:%x \n",ul,nul);

}
