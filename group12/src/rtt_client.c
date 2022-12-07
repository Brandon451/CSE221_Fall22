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

#define ITERS 1000
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void ping(int sockfd)
{
    char buff_send[MAX], buff_recv[MAX];
    memset(buff_send, 65, 64);
    write(sockfd, buff_send, sizeof(buff_send));
    read(sockfd, buff_recv, sizeof(buff_recv));
}
   
int main()
{
    for (int i = 0; i < ITERS; i++) 
    {
        int sockfd, connfd;
        struct sockaddr_in servaddr, cli;
    
        // Create Socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            exit(0);
        }
        bzero(&servaddr, sizeof(servaddr));
    
        //Assign Server
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("65.109.105.56");
        servaddr.sin_port = htons(PORT);
   
        // connect the client socket to server socket
        int connection = connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
        
        //* Code to be benchmarked
        start_benchmark();
        // function to send random 64B (to imitate ping)
        ping(sockfd);
        end_benchmark();
        //* Code End

        // close the socket
        close(sockfd);
        report_cycles(1);
    }
}
