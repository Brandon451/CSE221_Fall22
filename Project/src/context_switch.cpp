#include <stdio.h>
#include <stdlib.h>
#include <benchmark.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <sched.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <string.h>

#define RW_PROCESS 0
#define WR_PROCESS 1

inline void wrProcess(int iterations, int *pipe1_fds, int *pipe2_fds)
{
    bool inbuf[1];
    bool data = true;
    for (int i = 0; i < iterations; i++)
    {
        write(pipe1_fds[1], &data, 1);
        read(pipe2_fds[0], inbuf, 1);
    }
    close(pipe1_fds[1]);
}

inline void rwProcess(int iterations, int *pipe1_fds, int *pipe2_fds)
{
    bool inbuf[1];
    bool data = true;
    for (int i = 0; i < iterations; i++)
    {
        read(pipe1_fds[0], inbuf, 1);
        //std::cout << " rw got " << inbuf[0] << std::endl;
        write(pipe2_fds[1], &data, 1);
        //std::cout << " rw sent " << data << std::endl;
    }
    close(pipe2_fds[1]);
}

int benchmark_process_context_switch(int cores, int iterations, bool warmup, int overheads)
{
    pid_t pid;
    std::vector<int *> pipes;
    overhead(overheads);
    // Generate pipes for each core
    for (int i = 0; i < cores * 2; i++)
    {
        pipes.push_back(new int[2]);
        //create a pipe
        if (pipe(pipes[i]))
        {
            return EXIT_FAILURE;
        }
    }
    // Create a wr process for each core
    int type = WR_PROCESS;
    int pipe_index;
    for (int i = 0; i < cores; i++)
    {
        pipe_index = i;
        pid = fork();
        if (pid == 0)
            break;
    }
    if (pid > 0)
    {
        // Create a rw process for each core.
        // Parent process will be a rw process too
        type = RW_PROCESS;
        for (int i = 0; i < cores - 1; i++)
        {
            pipe_index = i;
            pid = fork();
            if (pid == 0)
                break;
            pipe_index = cores - 1;
        }
    }
    // Only parent process starts measurement
    if (pid > 0)
    {
        start_benchmark();
    }
    // Processes execute their own work
    if (type == WR_PROCESS)
    {
        wrProcess(iterations, pipes[pipe_index], pipes[pipe_index + cores]);
    }
    else
    {
        rwProcess(iterations, pipes[pipe_index], pipes[pipe_index + cores]);
    }
    // Fin
    if (pid > 0)
    {
        wait(nullptr);
        end_benchmark();
        if(!warmup)
            report_cycles(iterations*2);
    }
    else
    {
        //terminate child processes
        exit(0);
    }
    return EXIT_SUCCESS;
}

struct thread_start_args
{
    int thread_type;
    int iterations;
    int *pipe1_fds;
    int *pipe2_fds;
};

void *thread_start(void *arg)
{
    struct thread_start_args *targs = (thread_start_args *)arg;
    if (targs->thread_type == RW_PROCESS)
    {
        rwProcess(targs->iterations, targs->pipe1_fds, targs->pipe2_fds);
    }
    else
    {
        wrProcess(targs->iterations, targs->pipe1_fds, targs->pipe2_fds);
    }
    return arg;
}

int benchmark_threads_context_switch(int iterations, bool warmup, int overheads)
{
    pthread_t rwthread, wrthread;
    int *pipe1_fds = new int[2];
    int *pipe2_fds = new int[2];
    struct thread_start_args *targs1 = new thread_start_args;
    targs1->thread_type = RW_PROCESS;
    targs1->iterations = iterations;
    targs1->pipe1_fds = pipe1_fds;
    targs1->pipe2_fds = pipe2_fds;
    overhead(overheads);
    if (pipe(targs1->pipe1_fds))
    {
        return EXIT_FAILURE;
    }
    if (pipe(targs1->pipe2_fds))
    {
        return EXIT_FAILURE;
    }
    pthread_create(&rwthread, NULL, &thread_start, targs1);
    struct thread_start_args *targs2 = new thread_start_args;
    targs2->thread_type = WR_PROCESS;
    targs2->iterations = iterations;
    targs2->pipe1_fds = pipe1_fds;
    targs2->pipe2_fds = pipe2_fds;
    pthread_create(&wrthread, NULL, &thread_start, targs2);
    start_benchmark();
    pthread_join(rwthread, NULL);
    pthread_join(wrthread, NULL);
    end_benchmark();
    if(!warmup)
        report_cycles(iterations*2);
    delete targs1, targs2, pipe1_fds, pipe2_fds;
    return EXIT_SUCCESS;
}

int benchmark_wr_overhead(int iterations, bool warmup){
    int *pipe1_fds = new int[2];
    if (pipe(pipe1_fds))
    {
        return EXIT_FAILURE;
    }
    //this measurement will account for loop overhead + measurement overhead
    overhead(0);
    start_benchmark();
    wrProcess(iterations, pipe1_fds, pipe1_fds);
    end_benchmark();
    if(!warmup)
        report_cycles(iterations*2);
    delete pipe1_fds;
    return EXIT_SUCCESS; 
}

int main(int argc, char **argv)
{
    cpu_set_t cpu_mask;
    CPU_ZERO(&cpu_mask);
    CPU_SET(0, &cpu_mask);
    //set cpu affinity so that process runs only on single core
    sched_setaffinity(getpid(), sizeof(cpu_mask), &cpu_mask);
    //ensure that this process gets the highest priority
    setpriority(PRIO_PROCESS, getpid(), -20);
    if (argc == 2)
    {
        if (strcmp(argv[1], "process") == 0)
        {
            //warmup
            benchmark_process_context_switch(1, 10, true, 0);
            //main benchmark
            return benchmark_process_context_switch(1, 1000000, false, 450);
        }
        if (strcmp(argv[1], "kernel_thread") == 0)
        {
            //warmup
            benchmark_threads_context_switch(10, true, 0);
            //main benchmark
            return benchmark_threads_context_switch(1000000, false, 450);
        }
        if (strcmp(argv[1], "wr_overhead") == 0)
        {
            //warmup
            benchmark_wr_overhead(10, true);
            //main benchmark
            return benchmark_wr_overhead(10, false);
        }
    }
}
