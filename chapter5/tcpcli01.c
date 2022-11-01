#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <strings.h>

#define SERVPORT 8860

int main(int argc, char *argv[])
{   
    int sockfd;
    struct sockaddr_in servaddr;
    if (argc != 2){
        fprintf(stderr, "usage: ./tcpcli01 <IPADDRESS>");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("socket()");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    
}