
// tcpserv01.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#include "myunp.h"

#define SERVPORT 8860
#define LISTENQ  128


void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0){
        printf("child %d terminated\n", pid);
    }

    return;
}


int main(int argc, char *argv[])
{
    int listenfd, connfd;
    int ret;
    char str[INET_ADDRSTRLEN];
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    signal(SIGCHLD, sig_chld);


    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0){
        perror("socket()");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);

    ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret < 0){
        perror("bind()");
        exit(1);
    }

    ret = listen(listenfd, LISTENQ);
    if (ret < 0){
        perror("listen()");
        exit(1);
    }

    for (; ;){
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0){
            if (errno == EINTR ){
                continue;
            } else {
                perror("accept()");
                exit(1);
            }
        }
        printf("Accept client: %s at port: %d\n", 
                inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
                ntohs(cliaddr.sin_port));

        if ((childpid = fork()) == 0){
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }
    exit(0);
}
