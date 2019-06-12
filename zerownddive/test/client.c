#include <sys/types.h>   
#include <sys/socket.h>   
#include <sys/time.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>   
#include <errno.h>   
#include <fcntl.h>   
#include <netdb.h>   
#include <signal.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
#include <sys/stat.h>   
#include <sys/uio.h>   
#include <unistd.h>   
#include <sys/wait.h>   
#include <sys/un.h>   
#include <sys/time.h>   
#define LISTING     (1024)   
#define BUFFSIZE    (1024)   
#define MAXLINE     (1024*4)   
#define SERVERIP    "10.216.25.45"   
#define SERVERPORT  12345
#define SA      struct sockaddr   
char cRcvBuffer[BUFFSIZE];   
int Socket(int family, int type, int protocol)    
{   
	int n = 0;   
	if ((n = socket(family, type, protocol)) < 0) {   
		fprintf(stderr, "socket error\n");   
}   
return (n);   
}   
int main(int argc, char *argv[])   
{   
	int iConnFD;   
	int iLen = 0;   
	int iReuse = 1;   
	int i;

	struct sockaddr_in   SerAddr;   
	iConnFD = Socket(AF_INET, SOCK_STREAM, 0);   
	bzero(&SerAddr, sizeof(SerAddr));   
	SerAddr.sin_family = AF_INET;   
	SerAddr.sin_port = htons(SERVERPORT);   
	setsockopt(iConnFD , SOL_SOCKET, SO_REUSEADDR, &iReuse, sizeof(int));   
	inet_pton(AF_INET,SERVERIP,&SerAddr.sin_addr);   
	if (connect(iConnFD , (SA*)&SerAddr, sizeof(SerAddr)) < 0) {   
		fprintf(stderr,"connect error.\n");   
	}   

	send(iConnFD , "Hello Here is a Network test.",32,0);   
#if 1
	for (i = 0; i < 100; i++)
	{while ((iLen += read(iConnFD , cRcvBuffer, 1000))>0) {   
		printf("RCV:%d\n",iLen);   
		sleep(5);
	}   
	sleep(5);
	}
#endif   
	close(iConnFD);   
	return 0;   
}  
