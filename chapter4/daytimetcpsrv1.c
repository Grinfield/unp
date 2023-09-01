// daytimetcpsrv1.c
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>

#define SRVPORT 8880
#define BACKLOG 128

int main(int argc, char *argv[])
{
    int listenfd, connfd;
    int ret;
    socklen_t len;
    char buff[BUFSIZ];
    time_t t;
    struct sockaddr_in servaddr, cliaddr, tmpaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0){
        perror("socket()");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SRVPORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret < 0){
        perror("bind()");
        exit(1);
    }

    ret = listen(listenfd, BACKLOG);
    if (ret < 0){
        perror("listen()");
        exit(1);
    }

    for (; ;){
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
        if (connfd < 0){
            perror("accept()");
            exit(1);
        }
        printf("Connection from: %s, port: %d\n", 
                inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
                ntohs(cliaddr.sin_port));

        len = sizeof(tmpaddr);
        ret = getsockname(connfd, (struct sockaddr *)&tmpaddr, &len);
        if (ret < 0){
            perror("getsockname()");
            exit(1);
        }
        printf("Server address: %s, port: %d\n", 
                inet_ntop(AF_INET, &tmpaddr.sin_addr, buff, sizeof(buff)),
                ntohs(tmpaddr.sin_port));

        t = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&t));
        ret = write(connfd, buff, strlen(buff));
        if (ret < 0){
            perror("write()");
            exit(1);
        }
        shutdown(connfd, SHUT_RDWR);
        close(connfd);
    }

    exit(0);
}
