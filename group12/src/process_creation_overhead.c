#include <benchmark.h>
#include <procedures.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_ITERATIONS 100000

int main() {
    //warmup
    if(fork());
    else return 0;
    //benchmark
    //measurement overhead 
    overhead(0);
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        start_benchmark();
        if(fork());     // TODO: account for overhead caused by if()
        else return 0;
        end_benchmark();
        report_cycles(1);
    }
}