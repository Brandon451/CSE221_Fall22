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
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<math.h>

#define SIZE_OF_STAT 10 
#define BOUND_OF_LOOP 10
#define SIZE_OF_FILE 1073741824
#define UINT64_MAX (18446744073709551615ULL)

int main(int argc, char* argv[])
{
	unsigned int cycles_low, cycles_high, cycles_low1, cycles_high1, index;
	unsigned long long int start, end;

	if (argc < 2){
		printf("File sizes not supplied.\n");
	}

    // File Stuff
	// Random File Create: dd if=/dev/urandom of=512_mb.txt bs=64M count=16 iflag=fullblock
    char file_create[300];
    char *file_name = "fileread_local.txt";
	int size = atoi(argv[1]) * pow(1024,atoi(argv[2]));
	int count = 1;
    sprintf(file_create, "mkdir ./files; dd if=/dev/urandom of=%s bs=%d count=%d iflag=fullblock ; mv %s ./files/%s", file_name, size, count, file_name, file_name);
	system(file_create);

    char filepath[500];
    sprintf (filepath, "./files/%s", file_name);
    int fd = open(filepath,O_RDONLY|O_DIRECT);
	if (fd == -1){
		printf("Error with open: %s\n", strerror(errno));
	}
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
		
	// Clear file buffer cache	
	system("sync; echo 1 > /proc/sys/vm/drop_caches");
	// Create aligned buffer for file contents
	void *buffer; 
	int aligned = posix_memalign(&buffer, 4096, 4096);
	if (aligned != 0){
		printf("Alignment Error\n");
	}
	
	size_t buf_size = 4096; //sizeof(buffer);
	int nbytes;
	srand(420);

	start_benchmark();
	//** Code to be Benchmarked
	for(int i = 0; i < (size / 4096) ; i++) 
	{  	
		// Random offset in range 0 - 4096	
		int block_num = rand() % ((size/4096)-1);
		int offset = block_num*4096;
		lseek(fd, offset, SEEK_SET);
        nbytes = read(fd, buffer, buf_size);				
	}
	//** Code End
	end_benchmark();
	double c1 = report_cycles((size / 4096));

	if (nbytes == -1){
		printf("nbytes:%d, Error: %s\n",nbytes, strerror(errno));
	}

	start_benchmark();
	//** Code to be Benchmarked
	for(int i = 0; i < (size / 4096); i++){
		lseek(fd, (rand() % (size/4096)), SEEK_SET);
	}
	//** Code End
	end_benchmark();
	double c2 = report_cycles((size / 4096));

	report_difference(c1, c2);

	close(fd);
	system("rm -r ./files");
	return 0;
}