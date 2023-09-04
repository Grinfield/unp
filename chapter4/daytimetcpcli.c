#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>

int main(int argc, char *argv[])
{
    int clisock, n, ret;
    char linebuff[BUFSIZ];
    struct sockaddr_in servaddr;

    if (argc != 3){
        fprintf(stderr, "usage: %s <server ip> <server port>\n", argv[0]);
        exit(1);
    }

    clisock = socket(AF_INET, SOCK_STREAM, 0);
    if(clisock < 0){
        perror("socket()");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        perror("inet_pton()");
        exit(1);
    }

    ret = connect(clisock, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret < 0){
        perror("connect()");
        exit(1);
    }

    while((n = read(clisock, linebuff, BUFSIZ)) > 0){
        linebuff[n] = '\0';
        if (fputs(linebuff, stdout) == EOF){
            perror("fputs()");
            exit(1);
        }
    }
    if (n < 0){
        perror("read()");
        exit(1);
    }

    exit(0);
}