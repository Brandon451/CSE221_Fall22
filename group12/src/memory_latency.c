#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/resource.h>
#include <unistd.h>
#include "benchmark.h"

int* random_alloc(int size, int stride){
    int* buf = (int*)calloc(size, sizeof(int));
    int val, i, allocs;
    int index;
    srand(500);
    int flip = 1;
    val = (stride*(rand() % size))%(size/2);
    val = (flip*(size/2)) + val;
    index = 0;
    i = 0;
    allocs = size/stride;
    while(i < allocs-1){
        while(buf[val] != 0 || index == val){
            val = (stride*(rand() % size))%(size/2);
            val = (flip*(size/2)) + val;
        }
        buf[index] = val;
        index = val; 
        flip = 1-flip;
        val = (stride*(rand() % size))%(size/2);
        val = (flip*(size/2)) + val;
        i++;
    }
    buf[index] = 0;
    return buf;
}


volatile __attribute__ ((noinline)) int benchmark_latency(int size, int stride, int iterations){

    int val, i, allocs;
    int index;
    int* buf = random_alloc(size, stride);
    i = 0;
    index = 0;
    int acc = 0;
    start_benchmark();
    while(i<iterations){
        index = buf[index];
        i++;
    }
    end_benchmark();
    report_cycles(iterations);
    free(buf);
    return 0;
}

int main(int argc, char** argv){
    long long unsigned int size;
    unsigned int stride;
    unsigned int iterations;
    int opt;
    while((opt = getopt(argc, argv, ":n:s:i:")) != -1){
        switch(opt){
            case 'n':
                size = atoi(optarg);
                break;
            case 's':
                stride = atoi(optarg);
                break;
            case 'i':
                iterations = atoi(optarg);
                break;
            case '?':
                size = 128;
                stride = 4;
                iterations = 10;
                printf("Arguments not supplied correctly, going with default \n");
                break;
        }
    }
    cpu_set_t cpu_mask;
    CPU_ZERO(&cpu_mask);
    CPU_SET(0, &cpu_mask);
    //set cpu affinity so that process runs only on single core
    sched_setaffinity(getpid(), sizeof(cpu_mask), &cpu_mask);
    //ensure that this process gets the highest priority
    setpriority(PRIO_PROCESS, getpid(), -20);
    benchmark_latency(size, stride, iterations);
    return 0;
}
