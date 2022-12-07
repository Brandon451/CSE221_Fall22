#ifndef _GNU_SOURCE
	#define _GNU_SOURCE 
#endif
#include<stdio.h>
#include<stdlib.h>
#include<sched.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<benchmark.h>
#include<sys/mman.h>
#include <fcntl.h>

#define SIZE_OF_FILE 1073741824
#define ITERS 1000

int main()
{
	unsigned int cycles_low, cycles_high, cycles_low1, cycles_high1, index;
	unsigned long long int start, end;

	system("mkdir ./files; dd if=/dev/urandom of=1_gb.txt bs=64M count=16 iflag=fullblock ; mv 1_gb.txt ./files/1_gb.txt");
        void *file_pointer;
	char first;
        const char *filepath = "./files/1_gb.txt";
        int fd = open(filepath,O_RDONLY);
	posix_fadvise64(fd,0, SIZE_OF_FILE, POSIX_MADV_DONTNEED);


	cpu_set_t cpu_mask;
	CPU_ZERO(&cpu_mask);
	CPU_SET(0, &cpu_mask); 
	//set cpu affinity so that process runs only on single core
	sched_setaffinity(getpid(), sizeof(cpu_mask), &cpu_mask);
	
	//ensure that this process gets the highest priority
	setpriority(PRIO_PROCESS, getpid(), -20);

	asm volatile("cpuid\n\t"
                "rdtsc\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t"
                : "=r" (cycles_high), "=r" (cycles_low)
                :: "%rax", "%rbx", "%rcx", "%rdx"); 
			
	asm   volatile("RDTSCP\n\t"          
			"mov %%edx, %0\n\t"          
			"mov %%eax, %1\n\t"          
			"CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: 
			"%rax", "%rbx", "%rcx", "%rdx"); 
			
	file_pointer = mmap(NULL, SIZE_OF_FILE, PROT_READ, MAP_SHARED, fd,0); 
	first = ((char*)file_pointer)[0];
	munmap(file_pointer,SIZE_OF_FILE);

	srand(500);
	index = 1;

	for(int i = 0; i < ITERS; i++) 
	{  
		system("sync; echo 1 > sudo /proc/sys/vm/drop_caches");
		// mmap file     
		index = index + (100+(rand()%100))*getpagesize();
		file_pointer = mmap(NULL, SIZE_OF_FILE, PROT_READ, MAP_SHARED, fd,0); 
		start_benchmark();						
	    //** Code to be Benchmarked
		// attempt to access initial item of file
		first = ((char*) file_pointer)[index];
		//** Code End
		end_benchmark();
		report_cycles(1);
		// unmap file
		munmap(file_pointer, SIZE_OF_FILE);				
	}
	close(fd);
	system("rm -r ./files");
	return 0;
}
