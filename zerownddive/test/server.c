#include  <unistd.h>
#include  <sys/types.h>       /* basic system data types */
#include  <sys/socket.h>      /* basic socket definitions */
#include  <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include  <arpa/inet.h>       /* inet(3) functions */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 1024
//typedef struct sockaddr  SA;
void handle(int connfd);

int  main(int argc, char **argv)
{
    int     listenfd, connfd;
    int  serverPort = 12345;
    int listenq = 1024;
    pid_t   childpid;
    char buf[MAXLINE];
    socklen_t socklen;

    struct sockaddr_in cliaddr, servaddr;
    socklen = sizeof(cliaddr);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(serverPort);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket error");
        return -1;
    }
    if (bind(listenfd, (struct sockaddr *) &servaddr, socklen) < 0) {
        perror("bind error");
        return -1;
    }
    if (listen(listenfd, listenq) < 0) {
        perror("listen error");    
        return -1;
    }
    printf("echo server startup,listen on port:%d\n", serverPort);
    for ( ; ; )  {
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &socklen);
        if (connfd < 0) {
            perror("accept error");
            continue;
        }

        sprintf(buf, "accept form %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
        printf(buf,"");
        childpid = fork();
        if (childpid == 0) { /* child process */
            close(listenfd);    /* close listening socket */
            handle(connfd);   /* process the request */
            exit (0);
        } else if (childpid > 0)  {
            close(connfd);          /* parent closes connected socket */
        } else {
            perror("fork error");
        }
    }
    return 0;
}


void handle(int connfd)
{
    int i;
    int send_len = 0;
    size_t n;
    char    buf[MAXLINE];
    char buf2[100000];
    for (i = 0; i < 100000; i++)
	    buf2[i] = 'a';
    buf2[99999]='\0';

    sleep(1);
    n = read(connfd, buf, MAXLINE);
    if (n != 0)
	    printf("rcv: %s\n", buf);
    for (i = 0; i< 100; i++) {
	    //write(connfd, buf2, 100);
	   send_len += send(connfd, buf2, 1000, 0);
	    printf("send: %d\n", send_len);
		sleep(1);
    }
    return;
    for(;;) {
        n = read(connfd, buf, MAXLINE);
	printf("rcv: %d\n", n);
        if (n < 0) {
            if(errno != EINTR) {
                perror("read error");
                break;
            }
        }
        if (n == 0) {
            //connfd is closed by client
            close(connfd);
            printf("client exit\n");
            break;
        }
        //client exit
        if (strncmp("exit", buf, 4) == 0) {
            close(connfd);
            printf("client exit\n");
            break;
        }
    } 
} 
