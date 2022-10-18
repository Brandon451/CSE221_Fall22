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
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SIZE_OF_STAT 100
#define BOUND_OF_LOOP 10
#define UINT64_MAX (18446744073709551615ULL)
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buff_snd[MAX], buff_recv[MAX];
    memset(buff_snd, 65, 64);
    write(sockfd, buff_snd, sizeof(buff_snd));
    read(sockfd, buff_recv, sizeof(buff_recv));
}
   
int main()
{
    /* Client sends a 64 byte message to the server, server echoes the message
    / Time is measured between when the client connects to the server and when it receives the 
    / echoed message */
    for (int i = 0; i < BOUND_OF_LOOP * SIZE_OF_STAT ; i++) 
    {
        int sockfd, connfd;
        struct sockaddr_in servaddr, cli;
    
        // socket create and verification
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            exit(0);
        }
        bzero(&servaddr, sizeof(servaddr));
    
        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(PORT);
   
        // connect the client socket to server socket
        int conn = connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
        
        //* Code to be benchmarked
        start_benchmark();
        // function to send random 64B (to imitate ping)
        func(sockfd);
        end_benchmark();
        //* Code End

        // close the socket
        close(sockfd);
        report_cycles(1);
    }
}