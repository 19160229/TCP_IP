#include "winsock2.h"
#include "ws2tcpip.h"

#include "stdio.h"
#include "stdlib.h"



#define IP_RECORD_ROUTE  0x7

// 
// IP ��ͷ�ṹ����
//

typedef struct _iphdr 
{
    unsigned char  ip_verlen;      // IP�汾����ͷ��
    unsigned char  tos;            // TOS
    unsigned short total_len;      // ���ܳ�
    unsigned short ident;          // ID
    unsigned short frag_and_flags; // ���
    unsigned char  ttl;            // TTL
    unsigned char  proto;          // Э�� (TCP, UDP ��)
    unsigned short checksum;       // IP У���

    unsigned int   sourceIP;
    unsigned int   destIP;
} IpHeader;






#define ICMP_ECHO        8
#define ICMP_ECHOREPLY   0
#define ICMP_MIN         8 // ������Ӧ�����С��ICMP���ĳ��ȣ�8-byte ICMP ��ͷ

//
// ICMP ��ͷ
//
typedef struct _icmphdr 
{
    BYTE   i_type;
    BYTE   i_code;                
    USHORT i_cksum;
    USHORT i_id;
    USHORT i_seq;
    // �Լ������ICMP������
    ULONG  timestamp;
} IcmpHeader;

//
// IP ͷ��ѡ��- ���� IP_OPTIONS
//
typedef struct _ipoptionhdr
{
    unsigned char        code;        // ѡ������
    unsigned char        len;         // ѡ��ͷ��
    unsigned char        ptr;         // ѡ��ƫ��
    unsigned long        addr[9];     // IP��ַ�б�
} IpOptionHeader;

#define DEF_PACKET_SIZE  32        
#define MAX_PACKET       1024      // ���� ICMP ����
#define MAX_IP_HDR_SIZE  60        // ���� IP ͷ��

BOOL  bRecordRoute;
int   datasize;
char *lpdest;

//��ӡ�����ʹ�÷���
void usage(char *progname)
{
    printf("�÷�: ping -r <host> [data size]\n");
    printf("       -r           ��¼·��\n");
    printf("        host        Ҫping��Զ�̻�����\n");
    printf("        datasize    ���ݴ�С�����1K\n");
    ExitProcess(-1);
}

//���ICMP����
void FillICMPData(char *icmp_data, int datasize)
{
    IcmpHeader *icmp_hdr = NULL;
    char       *datapart = NULL;

    icmp_hdr = (IcmpHeader*)icmp_data;
    icmp_hdr->i_type = ICMP_ECHO;        // ICMP��ѯ������
    icmp_hdr->i_code = 0;
    icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_seq = 0;
  
    datapart = icmp_data + sizeof(IcmpHeader);
    //
    // �������
    //
    memset(datapart,'E', datasize - sizeof(IcmpHeader));
}

//����У���
USHORT checksum(USHORT *buffer, int size) 
{
    unsigned long cksum=0;

    while (size > 1) 
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size) 
    {
        cksum += *(UCHAR*)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return (USHORT)(~cksum);
}

//IPѡ��Ĵ���:��¼·��ѡ��
void DecodeIPOptions(char *buf, int bytes)
{
    IpOptionHeader *ipopt = NULL;
    IN_ADDR         inaddr;
    int             i;
    HOSTENT        *host = NULL;

    ipopt = (IpOptionHeader *)(buf + 20);

    printf("RR:   ");
    for(i = 0; i < (ipopt->ptr / 4) - 1; i++)
    {
        inaddr.S_un.S_addr = ipopt->addr[i];
        if (i != 0)
            printf("      ");
        host = gethostbyaddr((char *)&inaddr.S_un.S_addr,
                    sizeof(inaddr.S_un.S_addr), AF_INET);
        if (host)
            printf("(%-15s) %s\n", inet_ntoa(inaddr), host->h_name);
        else
            printf("(%-15s)\n", inet_ntoa(inaddr));
    }
    return;
}

//ICMPͷ��ѡ���
void DecodeICMPHeader(char *buf, int bytes,  struct sockaddr_in *from)
{
    IpHeader       *iphdr = NULL;
    IcmpHeader     *icmphdr = NULL;
    unsigned short  iphdrlen;
    DWORD           tick;
    static   int    icmpcount = 0;

    iphdr = (IpHeader *)buf;
	// ��32λΪһ����λ����Ҫ*4
    iphdrlen = (iphdr->ip_verlen&0x0000000f) * 4;
    tick = GetTickCount();

    if ((iphdrlen == MAX_IP_HDR_SIZE) && (!icmpcount))
        DecodeIPOptions(buf, bytes);

    if (bytes  < iphdrlen + ICMP_MIN) 
    {
        printf("�� %s ����������\n", inet_ntoa(from->sin_addr));
    }
    icmphdr = (IcmpHeader*)(buf + iphdrlen);

    if (icmphdr->i_type != ICMP_ECHOREPLY) 
    {
        printf("���ǲ�ѯ��Ӧ����������:%d \n", icmphdr->i_type);
        return;
    }
    //��ʱ�ǲ�ѯ��Ӧ������
    if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) 
    {
        printf("�����˵�ICMP��!\n");
        return ;
    }
    printf("�� %s: �������� %d �ֽ� ", inet_ntoa(from->sin_addr),bytes);
    printf(" icmp_seq = %d. ", icmphdr->i_seq);
    printf(" ʱ��: %d ms", tick - icmphdr->timestamp);
    printf("\n");

    icmpcount++;
    return;
}

void ValidateArgs(int argc, char **argv)
{
    int                i;

    bRecordRoute = FALSE;
    lpdest = NULL;
    datasize = DEF_PACKET_SIZE;
    
    for(i = 1; i < argc; i++)
    {
        if ((argv[i][0] == '-') || (argv[i][0] == '/'))
        {
            switch (tolower(argv[i][1]))
            {
                case 'r':        // ��¼·��
                    bRecordRoute = TRUE;
                    break;
                default:
                    usage(argv[0]);
                    break;
            }
        }
        else if (isdigit(argv[i][0]))
            datasize = atoi(argv[i]);
        else
            lpdest = argv[i];
    }
}
        
//������
int main(int argc, char **argv)
{

    WSADATA            wsaData;
    SOCKET             sockRaw = INVALID_SOCKET;
    struct sockaddr_in dest,
                       from;
    int                bread,
                       fromlen = sizeof(from),
                       timeout = 1000,
                       ret;
    char              *icmp_data = NULL,
                      *recvbuf = NULL;
    unsigned int       addr = 0;
    USHORT             seq_no = 0;
    struct hostent    *hp = NULL;
    IpOptionHeader     ipopt;


    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed: %d\n", GetLastError());
        return -1;
    }
    ValidateArgs(argc, argv);

	sockRaw = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockRaw == INVALID_SOCKET) 
    {
        printf("WSASocket() failed: %d\n", WSAGetLastError());
        return -1;
    }
    if (bRecordRoute)
    {
        
        ZeroMemory(&ipopt, sizeof(ipopt));
        ipopt.code = IP_RECORD_ROUTE; // ��¼·��ѡ��
        ipopt.ptr  = 4;               // ָ���һ����ַ��
        ipopt.len  = 39;              // ѡ��ͷ����
  
        ret = setsockopt(sockRaw, IPPROTO_IP, IP_OPTIONS, 
            (char *)&ipopt, sizeof(ipopt));
        if (ret == SOCKET_ERROR)
        {
            printf("setsockopt(IP_OPTIONS) failed: %d\n", 
                WSAGetLastError());
        }
    }
    // ���ó�ʱѡ��
    //
	timeout=10000;
    bread = setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, 
                (char*)&timeout, sizeof(timeout));

    if(bread == SOCKET_ERROR) 
    {
        printf("setsockopt(SO_RCVTIMEO) failed: %d\n", 
            WSAGetLastError());
        return -1;
    }
    timeout = 10000;
    bread = setsockopt(sockRaw, SOL_SOCKET, SO_SNDTIMEO, 
                (char*)&timeout, sizeof(timeout));

    if (bread == SOCKET_ERROR) 
    {
        printf("setsockopt(SO_SNDTIMEO) failed: %d\n", 
            WSAGetLastError());
        return -1;
    }

    memset(&dest, 0, sizeof(dest));
    
    dest.sin_family = AF_INET;
    if ((dest.sin_addr.s_addr = inet_addr(lpdest)) == INADDR_NONE)
    {//���������������������ǵ��ʮ���Ʊ�ʾ����ʾ�ġ���
        if ((hp = gethostbyname(lpdest)) != NULL)
        {
            memcpy(&(dest.sin_addr), hp->h_addr, hp->h_length);
            dest.sin_family = hp->h_addrtype;
            printf("Ŀ�������IP[dest.sin_addr] = %s\n", inet_ntoa(dest.sin_addr));
        }
        else
        {
            printf("gethostbyname() failed: %d\n", 
                WSAGetLastError());
            return -1;
        }
    }        

    // 
    // ����һ��ICMP��
    //       
    datasize += sizeof(IcmpHeader);  //ICMP���ݣ�ͷ��

    icmp_data = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                  MAX_PACKET);//������
    recvbuf = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                  MAX_PACKET);//������
    if (!icmp_data) 
    {
        printf("HeapAlloc() failed: %d\n", GetLastError());
        return -1;
    }
    memset(icmp_data,0,MAX_PACKET);//���Ͱ��声
    FillICMPData(icmp_data,datasize);//���ICMP��ͷ:��ICMP��ѯ�������͡�
    //
    // ���ͺͽ���ICMP��
    //
    while(1) 
    {
        static int nCount = 0;
        int        bwrote;
                
        if (nCount++ == 4) //���ѭ��4��
            break;
                
        ((IcmpHeader*)icmp_data)->i_cksum = 0;
        ((IcmpHeader*)icmp_data)->timestamp = GetTickCount();
        ((IcmpHeader*)icmp_data)->i_seq = seq_no++;
        ((IcmpHeader*)icmp_data)->i_cksum = 
            checksum((USHORT*)icmp_data, datasize);

        //����ICMP����IP��ͷ��ϵͳ�Զ�����ȥ�ġ�
        bwrote = sendto(sockRaw, icmp_data, datasize, 0, 
                     (struct sockaddr*)&dest, sizeof(dest));
        if (bwrote == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
                printf("��������ʱ��ʱ��\n");
                continue;
            }
            printf("sendto() failed: %d\n", WSAGetLastError());
            return -1;
        }
        if (bwrote < datasize) 
        {
            printf("ֻд�� %d �ֽ�\n", bwrote);
        }

		//����IP��������IP��ͷ.�������ܵİ����˴�����Ϊ1K.
        bread = recvfrom(sockRaw, recvbuf, MAX_PACKET, 0, 
                    (struct sockaddr*)&from, &fromlen);
        if (bread == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
                printf("��������ʱ��ʱ��\n");
                continue;
            }
            printf("recvfrom() ʧ��: %d\n", WSAGetLastError());
            return -1;
        }

		//���յ���IP�����зֽ��ӡ��
		//�ȴ�ӡIP��ͷ��
		
		IpHeader  * iph = (IpHeader *)recvbuf;
		printf("IP�汾�ţ�%d\n",iph->ip_verlen  >> 4  & 0x0000000f);
        printf("IPͷ������%d\n",(iph->ip_verlen&0x0f) * 4);
        printf("IPЭ�飺��%d\n",iph->proto);
	  
        DecodeICMPHeader(recvbuf, bread, &from);
        printf("\n\n"); 
        Sleep(1000);
    }
    // �ͷ���Դ
    //
    if (sockRaw != INVALID_SOCKET) 
        closesocket(sockRaw);
    HeapFree(GetProcessHeap(), 0, recvbuf);
    HeapFree(GetProcessHeap(), 0, icmp_data);

    WSACleanup();
    return 0;
}
