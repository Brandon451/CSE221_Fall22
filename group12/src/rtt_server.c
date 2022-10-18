#ifndef _GNU_SOURCE
	#define _GNU_SOURCE 
#endif
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <benchmark.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SIZE_OF_STAT 100
#define BOUND_OF_LOOP 10
#define UINT64_MAX (18446744073709551615ULL)
#define MAX 64
#define PORT 8080
#define SA struct sockaddr
   
// Function designed for chat between client and server.
void func(int connfd)
{
    char buff_recv[MAX];
    // read the message from client and copy it in buffer
    read(connfd, buff_recv, sizeof(buff_recv)); 
    // and send that buffer to client
    write(connfd, buff_recv, sizeof(buff_recv));
}

int main()
{
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli;
   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
   
    for(;;){
        // Accept the data packet from client and verification
        connfd = accept(sockfd, (SA*)&cli, &len);
        //Recv and send
        func(connfd);
    }

    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
   
    // After chatting close the socket
    close(sockfd);
}