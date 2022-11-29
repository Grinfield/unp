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
    while ((n = read(sockfd, buf, BUFSIZ)) > 0) {
        printf("read %ld %s", n, buf);
        write(sockfd, buf, n);
    }

    if ( n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        fprintf(stderr, "str_echo: read error");
}

void str_cli(FILE *stream, int sock)
{
    char sendline[BUFSIZ], recvline[BUFSIZ];
    size_t len = 0;

    while(fgets(sendline, BUFSIZ, stream) != NULL){
        write(sock, sendline, strlen(sendline));

        len = read(sock, recvline, BUFSIZ);
        if (len == 0) {
            fprintf(stderr, "str_cli: server terminated prematurely");
            break;
        } else if (len < 0) {
            perror("read()");
            break;
        } else 
            printf("Received %ld %s", len, recvline);
    }
}
