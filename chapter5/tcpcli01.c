// tcpcli01.c
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "myunp.h"

#define SERVPORT 8860


void sig_pipe(int signo)
{
    printf("Caught SIGPIPE\n");
}


int main(int argc, char *argv[])
{   
    int sockfd, ret;
    struct sockaddr_in servaddr;

    if (argc != 2){
        fprintf(stderr, "usage: %s <IPADDRESS>", argv[0]);
        exit(1);
    }

    signal(SIGPIPE, sig_pipe);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("socket()");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVPORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret < 0){
        perror("connect()");
        exit(1);
    }

    // communicate with server
    str_cli(stdin, sockfd);

    exit(0);
}