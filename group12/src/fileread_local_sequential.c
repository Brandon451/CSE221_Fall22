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
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<math.h>


int main(int argc, char* argv[]) // args: bs, count 
{
	unsigned int cycles_low, cycles_high, cycles_low1, cycles_high1, index;

	if (argc < 2){
		printf("File size not entered. Please rerun with correct file size.\n");
	}

    char file_create[300];
    char *file_name = "fileread_local.txt";
    int size = atoi(argv[1]) * pow(1024,atoi(argv[2])); //Size of file
    int count = 1;

    sprintf(file_create, "dd if=/dev/urandom of=%s bs=%d count=%d iflag=fullblock ; mv %s ./files/%s", file_name, size, count, file_name, file_name);
    system(file_create);

    char filepath[500];
    sprintf (filepath, "./files/%s", file_name);
    int fd = open(filepath,O_RDONLY|O_DIRECT);
	if (fd == -1){
		printf("Error with open: %s\n", strerror(errno));
	}

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
	system("sync; echo 1 > sudo /proc/sys/vm/drop_caches");
	// Create aligned buffer for file contents
	void *buffer; 
	int aligned = posix_memalign(&buffer, 4096, 4096);
	if (aligned != 0){
		printf("Alignment Error\n");
	}
	
	size_t buf_size = 4096; //sizeof(buffer);
	int num_bytes;

	start_benchmark();
	for(int i = 0; i < (size/4096) ; i++) 
	{  		
        num_bytes = read(fd, buffer, buf_size);				
	}
	end_benchmark();
	report_cycles(size/4096);
	
	if (num_bytes == -1){
		printf("num_bytes:%d, Error: %s\n",num_bytes, strerror(errno));
	}

	close(fd);
	system("rm -r ./files");
	return 0;
}
