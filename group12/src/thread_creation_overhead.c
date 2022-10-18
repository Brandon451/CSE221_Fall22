#include <benchmark.h>
#include <procedures.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_ITERATIONS 1000

void *exit_thread(void *t) {
    pthread_exit(NULL);
}

int main() {
    pthread_t t;
    start_benchmark();
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        pthread_create(&t, NULL, exit_thread, (void *)i);
    }
    end_benchmark();
    report_cycles(NUM_ITERATIONS);
}