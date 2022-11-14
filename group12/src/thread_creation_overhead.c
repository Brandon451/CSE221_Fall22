#include <benchmark.h>
#include <procedures.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#define ITERS 1000

void *exit_thread(void *t) {
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    start_benchmark();
    for(int i = 0; i < ITERS; i++) {
        pthread_create(&thread, NULL, exit_thread, (void *)i);
    }
    end_benchmark();
    report_cycles(ITERS);
}
