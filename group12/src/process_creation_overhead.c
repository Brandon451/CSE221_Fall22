#include <benchmark.h>
#include <procedures.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define ITERS 100000

int main() {
    
    if(fork());
    else return 0;
    //benchmark
    //measurement overhead 
    overhead(0);
    for(int i = 0; i < ITERS; i++) {
        start_benchmark();
        if(fork());     // Process Creation
        else return 0;
        end_benchmark();
        report_cycles(1);
    }
}
