#ifndef _GNU_SOURCE
	#define _GNU_SOURCE 
#endif
#include <stdio.h>
#include <sched.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <benchmark.h>
#include <netdb.h>
#include <sys/socket.h>
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
        struct sockaddr_in servaddr, client;
    
        // Create Socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            exit(0);
        }
        bzero(&servaddr, sizeof(servaddr));
    
        // Server Assign
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("65.109.105.56");
        servaddr.sin_port = htons(PORT);

        // connect the client socket to server socket
        int connection = connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
        
        //* Code to be benchmarked
        start_benchmark();
        // close the socket
        close(sockfd);
        end_benchmark();
        //* Code End
        
        report_cycles(1);
    }
}
