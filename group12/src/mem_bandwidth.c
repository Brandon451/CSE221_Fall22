#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "benchmark.h"

#define CONVERT_BITS_TO_BYTES   0.125
#define CONVERT_BYTES_TO_MB     0.000000953674316

#define TIME_PER_CYCLE 1/(1.7 * 1000000000)

#define ll long long

#define READ_PER_CYCLE 2   // since we are dealing with a DDR4 RAM
#define BITS_PER_LINE   64  // 64-bit memory bus
#define NUM_INTERFACES  2

#define ITERS 1000
#define ARR_SIZE 1000   // let's fix our array size to 1000 MB

#define CACHE_SIZE (12 * 1024 * 1024)    // 12 MB L3 cache

#define READ_BANDWIDTH_FILE  "read_bandwidth.txt"
#define WRITE_BANDWIDTH_FILE  "write_bandwidth.txt"

char dummy_source[CACHE_SIZE], dummy_sink[CACHE_SIZE];

# define CLEAR_CACHE memcpy(dummy_sink, dummy_source, CACHE_SIZE)


void calculate_read_bandwidth() {
    FILE *read_bandwidth_file;
    read_bandwidth_file = fopen(READ_BANDWIDTH_FILE, "w");
	if (!read_bandwidth_file) {
		printf("Error opening file %s\n", READ_BANDWIDTH_FILE);
		exit(1);
	}

    ll array_size = ARR_SIZE * 1024 * 1024;
    ll num_elements = (ll)(array_size / sizeof(ll));
    ll *arr = (ll*)malloc(array_size);

    double read_bandwidth_sum = 0;

    for(int i = 0; i < ITERS; i++) {
        CLEAR_CACHE;
        ll *arr = (ll*)malloc(array_size);
        ll* start_ptr = arr;
        ll* end_ptr = arr + num_elements;

        start_benchmark();
        do {
            ll temp = *start_ptr;            //Reading
        } while(++start_ptr < end_ptr);
        end_benchmark();

        double read_cycles = report_cycles(1);
        double read_time_in_s = TIME_PER_CYCLE * read_cycles;
        double read_bandwidth = ((ARR_SIZE * 1024 * 1024 * 8)/read_time_in_s)  * READ_PER_CYCLE * BITS_PER_LINE * NUM_INTERFACES * CONVERT_BITS_TO_BYTES * CONVERT_BYTES_TO_MB;
        fprintf(read_bandwidth_file, "%lf", read_bandwidth);
        read_bandwidth_sum += read_bandwidth;

        free(arr);
    }

    double average_read_bandwidth = read_bandwidth_sum/ITERS;
    printf("Average read bandwidth with number of %d iterations): %.3lf\n", ITERS, average_read_bandwidth);
}

void calculate_write_bandwidth() {
    FILE *write_bandwidth_file;
    write_bandwidth_file = fopen(WRITE_BANDWIDTH_FILE, "w");
	if (!write_bandwidth_file) {
		printf("Error opening file %s\n", WRITE_BANDWIDTH_FILE);
		exit(1);
	}

    double write_bandwidth_sum = 0;

    ll array_size = ARR_SIZE * 1024 * 1024;
    ll num_elements = (ll)(array_size / sizeof(ll));
    ll *arr = (ll*)malloc(array_size);

    for(int i = 0; i < ITERS; i++) {
        CLEAR_CACHE;
        ll *arr = (ll*)malloc(array_size);
        ll* start_ptr = arr;
        ll* end_ptr = arr + num_elements;

        start_benchmark();
        do {
            *start_ptr = 1;                          //Writing
        } while(++start_ptr < end_ptr);
        end_benchmark();

        double write_cycles = report_cycles(1);
        double write_time_in_s = TIME_PER_CYCLE * write_cycles;
        double write_bandwidth = ((ARR_SIZE * 1024 * 1024 * 8)/write_time_in_s) * READ_PER_CYCLE * BITS_PER_LINE * NUM_INTERFACES * CONVERT_BITS_TO_BYTES * CONVERT_BYTES_TO_MB;
        fprintf(write_bandwidth_file, "%lf", write_bandwidth);
        write_bandwidth_sum += write_bandwidth;

        free(arr);
    }

    double average_write_bandwidth = write_bandwidth_sum/ITERS;
    printf("Average write bandwidth with number of %d iterations): %.3lf\n", ITERS, average_write_bandwidth);
}

int main(int argc, char const *argv[]) {
    calculate_read_bandwidth();
    calculate_write_bandwidth();
}
