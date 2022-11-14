#include <benchmark.h>
#include <procedures.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define ITERS 1000

int main(int argc, char const *argv[]) {
    start_benchmark();
    for(int i = 0; i < ITERS; i++) {
        procedure0();
    }
    end_benchmark();
    printf("Process0 ends\n");
    report_cycles(1);

    start_benchmark();
    for(int i = 0; i < ITERS; i++) {
        procedure1(1);
    }
    end_benchmark();
    printf("Process1 ends\n");
    report_cycles(1);

    start_benchmark();
    for(int i = 0; i < ITERS; i++) {
        procedure2(1, 1);
    }
    end_benchmark();
    printf("Process2 ends\n");
    report_cycles(1);

    start_benchmark();
    for(int i = 0; i < ITERS; i++) {
        procedure3(1, 1, 1);
    }
    end_benchmark();
    printf("Process3 ends\n");
    report_cycles(1);

    start_benchmark();
    for(int i = 0; i < ITERS; i++) {
        procedure4(1, 1, 1, 1);
    }
    end_benchmark();
    printf("Process4 ends\n");
    report_cycles(1);

    start_benchmark();
    for(int i = 0; i < ITERS; i++) {
        procedure5(1, 1, 1, 1, 1);
    }
    end_benchmark();
    printf("Process5 ends\n");
    report_cycles(1);

    start_benchmark();
    for(int i = 0; i < ITERS; i++) {
        procedure6(1, 1, 1, 1, 1, 1);
    }
    end_benchmark();
    printf("Process6 ends\n");
    report_cycles(1);

    start_benchmark();
    for(int i = 0; i < ITERS; i++) {
        procedure7(1, 1, 1, 1, 1, 1, 1);
    }
    end_benchmark();
    printf("Process7 ends\n");
    report_cycles(1);

    return 0;
}
