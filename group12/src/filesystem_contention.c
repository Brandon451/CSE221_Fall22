#ifndef _GNU_SOURCE
	#define _GNU_SOURCE 
#endif
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "benchmark.h"
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cassert>

void readblock(int fd, int blocks){
    char *buffer;
    posix_memalign((void **)&buffer, 4096, 8192);
    int i,j=0;
    int rb = 0;
    for(i =0; i<blocks; i++){
        rb = read(fd, buffer, 4096);
        if(rb < 0){
            perror("error reading file"); exit(1);
        }
        //printf("%d %d %d %d %d \n", rb, buffer[0], buffer[1], buffer[2], buffer[3]);
    }
    free(buffer);
}

void benchmark_contention(int num_processes, int n_blocks){
    int i;
    char cmd[] = "mkdir -p ./files; dd if=/dev/urandom of=files/file_contention_1.txt bs=64M count=2 iflag=fullblock";
    char filepath[] = "files/file_contention_1.txt";
    char rmcmd[] = "rm -rf ";
    int pid = 1;
    int fd;

    // create files for each of the processes
    for(i=0; i< num_processes; i++){
        // update cmd & filename depending on process
        cmd[62] = '0' + i;
        filepath[22] = '0' + i;
        system(cmd);
    }

    // clear file cache
    system("sync; echo 1 > /proc/sys/vm/drop_caches");

    // create processes
    for(i=0; i< num_processes-1; i++){
        // update filename depending on process
        filepath[22] = '0' + i;
        pid = fork();
        if(pid == 0){
            break;
        }
    }

    if(pid){
        //parent process
        // update cmd & filename depending on process
        filepath[22] = '0' + (num_processes-1);
        printf("%s\n",filepath);
        fd = open(filepath, O_RDONLY | O_DIRECT);
        if (fd < 0) { perror("error opening file"); exit(1); } 
	    posix_fadvise64(fd,0, 1073741824, POSIX_MADV_RANDOM);
        start_benchmark();
        readblock(fd, n_blocks);
        end_benchmark();
        report_cycles(n_blocks);
    }
    else{
        //child process
        fd = open(filepath, O_RDONLY | O_DIRECT);
        if (fd < 0) { perror("error opening file"); exit(1); } 
	    posix_fadvise64(fd,0, 1073741824, POSIX_MADV_RANDOM);
        readblock(fd, n_blocks);
    }
    close(fd);
    system(strcat(rmcmd, filepath));
}

int main(int argc, char** argv){
    int opt;
    int n_blocks;
    int num_processes;
    while((opt = getopt(argc, argv, ":n:b:")) != -1){
        switch(opt){
            case 'n':
                num_processes = atoi(optarg);
                break;
            case 'b':
                n_blocks = atoi(optarg);
                break;
            case ':':
                n_blocks = 10;
                num_processes = 8;
                break;
            case '?':
                n_blocks = 10;
                num_processes = 8;
                printf("Unsupported arg, going with default \n");
                break;
        }
    }
    benchmark_contention(num_processes, n_blocks);
    return 0;
}