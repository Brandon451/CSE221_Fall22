#include <stdio.h>
unsigned int start_cycles_high, start_cycles_low;
unsigned int end_cycles_high, end_cycles_low;
unsigned long long int start, end;
double benchmark_cycle_overhead;


inline void start_benchmark(){
    asm   volatile ("CPUID\n\t"       
            "RDTSC\n\t"       
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (start_cycles_high), "=r" (start_cycles_low)::
            "%rax", "%rbx", "%rcx", "%rdx"); 
}

inline void overhead(int overhead){
    benchmark_cycle_overhead = (double)overhead;
}

inline void end_benchmark(){
    asm   volatile("RDTSCP\n\t"                     
            "mov %%edx, %0\n\t"                     
            "mov %%eax, %1\n\t" "CPUID\n\t": "=r" (end_cycles_high), "=r" (end_cycles_low):: 
            "%rax", "%rbx", "%rcx", "%rdx");   
}

inline double report_cycles(int iterations){
        start = ( ((unsigned long long int)start_cycles_high << 32)   | start_cycles_low ); 
		end   = ( ((unsigned long long int)end_cycles_high << 32) | end_cycles_low );     
        double cycles = (double)((end-start)/iterations);
		printf("%.3f\n",  cycles - benchmark_cycle_overhead);
        return (cycles - benchmark_cycle_overhead);
}

inline double report_difference(double c1, double c2){
        return (c1 - c2);
}