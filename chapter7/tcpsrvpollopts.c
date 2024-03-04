// tcpsrvpollopts.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <poll.h>
#include <limits.h>

#include "myunp.h"

#define SERVPORT 8860
#define LISTENQ  128


int main(int argc, char *argv[])
{
    int i, n, ret, maxi;
    int listenfd, connfd, sockfd;

    long max_c = sysconf(_SC_OPEN_MAX);
    struct pollfd client[max_c];

    char buf[BUFSIZ];
    int nready;

    int val = 1;

    char dst_addr[INET_ADDRSTRLEN];
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

    // set SO_REUSEADDR opt
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0){
        perror("setsockopt()");
        exit(EXIT_FAILURE);
    }
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
    // set SO_KEEPALIVE opt
    if (setsockopt(listenfd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) < 0){
        perror("setsockopt()");
        exit(EXIT_FAILURE);
    }

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < max_c; i++)
        client[i].fd = -1;
    maxi = 0;

    for (; ;){
        if ((nready = poll(client, maxi + 1, -1)) < 0){
            perror("poll()");
            exit(EXIT_FAILURE);
        }

        if (client[0].revents & POLLRDNORM){
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

            for (i = 0; i < max_c; i++){
                if (client[i].fd < 0){
                    client[i].fd = connfd;
                    break;
                }
            }
            if (i == max_c){
                printf("too many clients\n");
                exit(EXIT_FAILURE);
            }

            client[i].events = POLLRDNORM;

            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;
        }

        // check all clients for data
        for ( i = 1; i <= maxi; i++) {
            if ((sockfd = client[i].fd) < 0)
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(sockfd, buf, BUFSIZ)) == 0){
                    close(sockfd);
                    client[i].fd = -1;
                }
                if (n > 0){
                    if (write(sockfd, buf, n) < 0) {
                        perror("write()");
                    }
                } else if (n < 0) {
                    if (errno == ECONNRESET){
                        close(sockfd);
                        client[i].fd = -1;
                    } else {
                        perror("read()");
                        exit(EXIT_FAILURE);
                    }
                }

                if (--nready <= 0)
                    break;
            }
        }
    }

    exit(EXIT_SUCCESS);
}
