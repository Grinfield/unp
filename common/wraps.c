#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "myunp.h"


void str_echo(int sockfd)
{
    ssize_t n;
    char buf[BUFSIZ];

again:
    while ((n = read(sockfd, buf, BUFSIZ - 1)) > 0) {
        buf[n] = '\0';
        printf("read %ld %s", n, buf);
        if(write(sockfd, buf, n) < 0){
            perror("write()");
            break;
        }
    }

    if ( n < 0 && errno == EINTR)
        goto again;
    else if (n < 0){
        perror("read()");
        fprintf(stderr, "str_echo: read error\n");
    }
}

#if 0
void str_cli(FILE *stream, int sock)
{
    char sendline[BUFSIZ], recvline[BUFSIZ];
    ssize_t len = 0;

    while (fgets(sendline, BUFSIZ, stream) != NULL) {
        if(write(sock, sendline, strlen(sendline)) < 0) {
            perror("write()");
            break;
        }

        len = read(sock, recvline, BUFSIZ);
        if (len == 0) {
            fprintf(stderr, "str_cli: server terminated prematurely\n");
            break;
        } else if (len < 0) {
            perror("read()");
            break;
        } else 
            printf("Received %ld %s", len, recvline);
    }
}
#endif
