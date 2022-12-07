#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <benchmark.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdint.h>
#include <float.h>

#define LOCAL "127.0.0.1"
#define SERVERADDR "65.109.105.56"
#define SERVERPORT 8080
#define KB 1024
#define MB (1024 * KB)
#define BLOCK (4 * KB)
#define ITERS 20
#define MAX_CONNS 1
#define DATASIZE (100 * MB)

#define TIME_PER_CYCLE 1/(1.7 * 1000000000)

int server() {
    char* buffer = (char*)malloc(BLOCK);

    int sock_server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, client_addr;

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sock_server, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(sock_server, MAX_CONNS);

    socklen_t client_len = sizeof(client_addr);

    while(1) {
        int sock_client = accept(sock_server, (struct sockaddr *)&server_addr, &client_len);
        int bytes_received = 0;
        while(1) {
            while((bytes_received = recv(sock_client, buffer, BLOCK, 0)) > 0);
            break;
        }

        close(sock_client);
    }
    close(sock_server);
    free(buffer);

    return 0;
}

void client() {
    char* data = (char *)malloc(DATASIZE);
    memset(data, '0', DATASIZE);

    int sock_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVERADDR);    // modify this for remote

    int r = connect(sock_client, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(r <0){
      printf("Error connecting! \n");
      exit(1);
    }
    double peak_time = DBL_MAX;
    for(int i = 0; i < ITERS; i++) {
        start_benchmark();
        int bytes_sent = send(sock_client, data, DATASIZE, 0);
        end_benchmark();
        double time = (report_cycles(1) * TIME_PER_CYCLE);
        peak_time = (time < peak_time) ? time : peak_time;
    }

    close(sock_client);
    printf("Peak bandwidth: %lf\n", DATASIZE / peak_time);
}

int main(int argc, char const *argv[]) {
    if(strcmp(argv[1], "s") == 0) {
        server();
    } else {
        client();
    }
    return 0;
}
