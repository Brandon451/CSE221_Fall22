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
#include<sys/mman.h>
#include <fcntl.h>
#include<stdlib.h>

#define SIZE_OF_STAT 10 
#define BOUND_OF_LOOP 10
#define SIZE_OF_FILE 1073741824
#define UINT64_MAX (18446744073709551615ULL)

int main()
{
	unsigned int cycles_low, cycles_high, cycles_low1, cycles_high1, index;
	unsigned long long int start, end;

    // File Stuff
	// Random File Create: dd if=/dev/urandom of=512_mb.txt bs=64M count=16 iflag=fullblock
	system("mkdir ./files; dd if=/dev/urandom of=1_gb.txt bs=64M count=16 iflag=fullblock ; mv 1_gb.txt ./files/1_gb.txt");
    void *fil;
	char first;
    const char *filepath = "./files/1_gb.txt";
    int fd = open(filepath,O_RDONLY);
	posix_fadvise64(fd,0, SIZE_OF_FILE, POSIX_MADV_DONTNEED);

    // End File Stuff

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
			
	fil = mmap(NULL, SIZE_OF_FILE, PROT_READ, MAP_SHARED, fd,0); 
	first = ((char*)fil)[0];
	munmap(fil,SIZE_OF_FILE);

	srand(420);
	index = 1;
	overhead(0);
	for(int i = 0; i < SIZE_OF_STAT * BOUND_OF_LOOP; i++) 
	{  
		system("sync; echo 1 > sudo /proc/sys/vm/drop_caches");
		// mmap file     
		index = index + (100+(rand()%100))*getpagesize();
		fil = mmap(NULL, SIZE_OF_FILE, PROT_READ, MAP_SHARED, fd,0); 
		start_benchmark();						
	    //** Code to be Benchmarked
		// attempt to access initial item of file
		first = ((char*) fil)[index];
		//** Code End
		end_benchmark();
		report_cycles(1);
		// unmap file
		munmap(fil, SIZE_OF_FILE);				
	}
	close(fd);
	system("rm -r ./files");
	return 0;
}
