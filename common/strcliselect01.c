#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

#include "myunp.h"

void str_cli(FILE *stream, int sock)
{
    int maxfdp1, stdineof, retval;
    fd_set rset;
    char sendline[BUFSIZ], recvline[BUFSIZ];
    ssize_t len = 0;

    stdineof = 0;
    FD_ZERO(&rset);

    for (; ;) {
        if (stdineof == 0)
            FD_SET(fileno(stream), &rset);
        FD_SET(sock, &rset);

        maxfdp1 = max(fileno(stream), sock) + 1;
        
        if ((retval = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0){
            perror("select()");
            exit(1);
        }
        // socket is readable
        if (FD_ISSET(sock, &rset)){
            len = read(sock, recvline, BUFSIZ - 1);
            if (len == 0) {
                if (stdineof == 1){
                    fprintf(stderr, "str_cli: server normal farewell\n");
                    break;
                } else {
                    fprintf(stderr, "str_cli: server terminated prematurely\n");
                    exit(1);
                }
            } else if (len < 0) {
                perror("read()");
                exit(1);
            } else {
                recvline[len] = '\0';
                printf("Received %ld %s", len, recvline);
            }
        }
        // input is readable
        if (stdineof == 0 && FD_ISSET(fileno(stream), &rset)){
            if ((len = read(fileno(stream), sendline, BUFSIZ)) == 0){
                stdineof = 1;
                printf("finished reading.\n");
                if (shutdown(sock, SHUT_WR) < 0){
                    perror("shutdown()");
                    // exit(1);
                }
                FD_CLR(sock, &rset);
                continue;
            }
            if (write(sock, sendline, len) < 0) {
                perror("write()");
                exit(1);
            }
        }
    }
}
