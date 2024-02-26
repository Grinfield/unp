// tcpsrvselect.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/select.h>

#include "myunp.h"

#define SERVPORT 8860
#define LISTENQ  128


int main(int argc, char *argv[])
{
    int i, n, ret, maxi, maxfd;
    int listenfd, connfd, sockfd;
    int client[FD_SETSIZE];

    fd_set rset, allset;
    char buf[BUFSIZ];
    int nready;

    char dst_addr[INET_ADDRSTRLEN];
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0){
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);

    ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret < 0){
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    ret = listen(listenfd, LISTENQ);
    if (ret < 0){
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    maxfd = listenfd;
    maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for (; ;){
        rset = allset;
        if ((nready = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0){
            perror("select()");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(listenfd, &rset)){
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
            if (connfd < 0) {
                if (errno == EINTR) {
                    continue;
                }
                else {
                    perror("accept()");
                    exit(EXIT_FAILURE);
                }
            }
            printf("Accept client: %s at port: %d\n", 
            inet_ntop(AF_INET, &cliaddr.sin_addr, dst_addr, sizeof(dst_addr)),
            ntohs(cliaddr.sin_port));

            for (i = 0; i < FD_SETSIZE; i++){
                if (client[i] < 0){
                    client[i] = connfd;
                    break;
                }
            }
            if (i == FD_SETSIZE){
                printf("too many clients\n");
                exit(EXIT_FAILURE);
            }

            FD_SET(connfd, &allset);
            if (connfd > maxfd)
                maxfd = connfd;
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;
        }

        for ( i = 0; i <= maxi; i++) {
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = read(sockfd, buf, BUFSIZ)) == 0){
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                if (n > 0){
                    if (write(sockfd, buf, n) < 0) {
                        perror("write()");
                    }
                } else if (n < 0) {
                    perror("read()");
                    fprintf(stderr, "read error\n");
                }

                if (--nready <= 0)
                    break;
            }
        }
    }

    exit(EXIT_SUCCESS);
}
