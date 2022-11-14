#ifndef _GNU_SOURCE
	#define _GNU_SOURCE 
#endif
#include<stdio.h>
#include<sched.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<benchmark.h>

#define ITERS 100 

int main()
{
	unsigned int cycles_low, cycles_high, cycles_low1, cycles_high1;
	unsigned long long int start, end;
	int child;

	cpu_set_t cpu_mask;
	CPU_ZERO(&cpu_mask);
	CPU_SET(0, &cpu_mask); 
	//set cpu affinity so that process runs only on single core
	sched_setaffinity(getpid(), sizeof(cpu_mask), &cpu_mask);
	
	//ensure that this process gets the highest priority
	setpriority(PRIO_PROCESS, getpid(), -20);


	for   (int i = 0; i < ITERS ; i++) 
	{       
		child = vfork(); 
		if (child ==  0)
		{
			start_benchmark();						
			//** Code to be Benchmarked
			getpid();
			//** Code End
			end_benchmark();
					
			report_cycles(1);
			exit(0);
		}
		
	}
	return 0;
}
