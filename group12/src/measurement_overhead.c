#ifndef _GNU_SOURCE
	#define _GNU_SOURCE 
#endif
#include<stdio.h>
#include<sched.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/resource.h>
#include <benchmark.h>

#define ITERS 1000

int main()
{
	unsigned int cycles_low, cycles_high, cycles_low1, cycles_high1;

	cpu_set_t cpu_mask;
	CPU_ZERO(&cpu_mask);
	CPU_SET(0, &cpu_mask); 
	//set cpu affinity so that process runs only on single core
	sched_setaffinity(getpid(), sizeof(cpu_mask), &cpu_mask);
	
	//ensure that this process gets the highest priority
	setpriority(PRIO_PROCESS, getpid(), -20);

	for   (int i = 0; i < ITERS ; i++) 
	{        
		start_benchmark();			
		//** Code to be Benchmarked
		asm   volatile ("CPUID\n\t"       
				"RDTSC\n\t"       
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
				"%rax", "%rbx", "%rcx", "%rdx"); 
		asm   volatile("RDTSCP\n\t"                     
				"mov %%edx, %0\n\t"                     
				"mov %%eax, %1\n\t" "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: 
				"%rax", "%rbx", "%rcx", "%rdx");  
		
		//** Code End
		end_benchmark();			
		report_cycles(1);
	}
	return 0;
}
