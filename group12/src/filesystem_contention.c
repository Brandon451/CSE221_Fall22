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


void read_block(int fd, int blocks){
    char *buffer;
    posix_memalign((void **)&buffer, 4096, 8192);
    int i,j=0;
    int read_buffer = 0;
    for(i =0; i<blocks; i++){
        read_buffer = read(fd, buffer, 4096);
        if(read_buffer < 0){
            perror("error reading file"); exit(1);
        }
    }
    free(buffer);
}

void create_contention(int num_processes, int num_blocks){
    int i;
    char cmd[] = "mkdir -p ./files; dd if=/dev/urandom of=files/file_contention_1.txt bs=64M count=2 iflag=fullblock";
    char filepath[] = "files/file_contention_1.txt";
    char rmcmd[] = "rm -rf ";
    int pid = 1;  //For Parent
    int fd;

    // Create file for each Process
    for(i=0; i< num_processes; i++){
        // update cmd & filename depending on process
        cmd[62] = '0' + i;
        filepath[22] = '0' + i;
        system(cmd);
    }

    // clear file cache
    system("sync; echo 1 > sudo /proc/sys/vm/drop_caches");

    // Create num_processes
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
        read_block(fd, num_blocks);
        end_benchmark();
        report_cycles(num_blocks);
    }
    else{
        //child process also readig the file
        fd = open(filepath, O_RDONLY | O_DIRECT);
        if (fd < 0) { perror("error opening file"); exit(1); } 
	  posix_fadvise64(fd,0, 1073741824, POSIX_MADV_RANDOM);
        read_block(fd, num_blocks);
    }
    close(fd);
    system(strcat(rmcmd, filepath));  //remove created files
}


int main(int argc, char** argv){
    int opt;
    int num_blocks;
    int num_processes;
    while((opt = getopt(argc, argv, ":n:b:")) != -1){
        switch(opt){
            case 'n':
                num_processes = atoi(optarg);
                break;
            case 'b':
                num_blocks = atoi(optarg);
                break;
            case '?':
                num_blocks = 10;
                num_processes = 8;
                printf("Arguments not supplied properly, going with default \n");
                break;
        }
    }
    create_contention(num_processes, num_blocks);
    return 0;
}
