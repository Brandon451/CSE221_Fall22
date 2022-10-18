#include <benchmark.h>
#include <procedures.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_ITERATIONS 1000

int main(int argc, char const *argv[]) {
    start_benchmark();
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        procedure0();
    }
    end_benchmark();
    report_cycles(NUM_ITERATIONS);

    start_benchmark();
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        procedure1(0);
    }
    end_benchmark();
    report_cycles(NUM_ITERATIONS);

    start_benchmark();
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        procedure2(0, 0);
    }
    end_benchmark();
    report_cycles(NUM_ITERATIONS);

    start_benchmark();
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        procedure3(0, 0, 0);
    }
    end_benchmark();
    report_cycles(NUM_ITERATIONS);

    start_benchmark();
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        procedure4(0, 0, 0, 0);
    }
    end_benchmark();
    report_cycles(NUM_ITERATIONS);

    start_benchmark();
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        procedure5(0, 0, 0, 0, 0);
    }
    end_benchmark();
    report_cycles(NUM_ITERATIONS);

    start_benchmark();
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        procedure6(0, 0, 0, 0, 0, 0);
    }
    end_benchmark();
    report_cycles(NUM_ITERATIONS);

    start_benchmark();
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        procedure7(0, 0, 0, 0, 0, 0, 0);
    }
    end_benchmark();
    report_cycles(NUM_ITERATIONS);

    return 0;
}
