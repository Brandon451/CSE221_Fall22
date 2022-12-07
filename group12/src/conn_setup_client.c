#ifndef _GNU_SOURCE
	#define _GNU_SOURCE 
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <netdb.h>
#include <sys/socket.h>
#include <benchmark.h>
#include <arpa/inet.h>

#define ITERS 1000
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
   
int main()
{
    for (int i = 0; i < ITERS ; i++) 
    {
        int sockfd, connfd;
        struct sockaddr_in servaddr, cli;
    
        // Create Socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            exit(0);
        }
        bzero(&servaddr, sizeof(servaddr));
    
        // Server Define
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("65.109.105.56");
        servaddr.sin_port = htons(PORT);

        //Code to be benchmarked
        start_benchmark();
        // connect the client socket to server socket
        int connection = connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
        end_benchmark();

        // close the socket
        close(sockfd);
        report_cycles(1);
    }
}
