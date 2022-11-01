#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

#include "myunp.h"


void str_echo(int sockfd)
{
    ssize_t n;
    char buf[BUFSIZ];

again:
    while ((n = read(sockfd, buf, BUFSIZ)) > 0)
        //printf("read %ld %s\n", n, buf);
        write(sockfd, buf, n);

    if ( n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        fprintf(stderr, "read error");
}


