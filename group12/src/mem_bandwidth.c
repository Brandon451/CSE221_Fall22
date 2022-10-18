#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "benchmark.h"

#define BITS_TO_BYTES   0.125
#define BYTES_TO_MB     0.000000953674316

#define TIME_PER_CYCLE 1/(1.7 * 1000000000)

#define ll long long

#define LINES_PER_CLOCK 2   // since we are dealing with a DDR4 RAM
#define BITS_PER_LINE   64  // 64-bit memory bus
#define NUM_INTERFACES  2

#define READ_BANDWIDTH_RECORD_FILE  "read_bandwidth.txt"
#define WRITE_BANDWIDTH_RECORD_FILE  "write_bandwidth.txt"

#define CACHE_SIZE (12 * 1024 * 1024)    // 12 MB L3 cache
char dummy_source[CACHE_SIZE], dummy_sink[CACHE_SIZE];
# define CLEAR_CACHE memcpy(dummy_sink, dummy_source, CACHE_SIZE)

#define NUM_ITERATIONS 1000
#define ARR_SIZE 1000   // let's fix our array size to 1000 MB

void measure_read_bandwidth() {
    FILE *read_bandwidth_file;
    read_bandwidth_file = fopen(READ_BANDWIDTH_RECORD_FILE, "w");
	if (!read_bandwidth_file) {
		printf("Error opening file %s\n", READ_BANDWIDTH_RECORD_FILE);
		exit(1);
	}

    ll arr_size = ARR_SIZE * 1024 * 1024;
    ll arr_elem_cnt = (ll)(arr_size / sizeof(ll));
    ll *arr = (ll*)malloc(arr_size);

    double aggregate_read_bandwidth = 0;

    for(int i = 0; i < NUM_ITERATIONS; i++) {
        CLEAR_CACHE;
        ll *arr = (ll*)malloc(arr_size);
        ll* start = arr;
        ll* end = arr + arr_elem_cnt;

        start_benchmark();
        do {
            ll temp = *start;
        } while(++start < end);
        end_benchmark();

        double read_cycles = report_cycles(1);
        // read_bandwidth calculcates bandwidth in MB/s
        double time_for_reads = TIME_PER_CYCLE * read_cycles;
        double read_bandwidth = ((ARR_SIZE * 1024 * 1024 * 8) / time_for_reads)  * LINES_PER_CLOCK * BITS_PER_LINE * NUM_INTERFACES * BITS_TO_BYTES * BYTES_TO_MB;
        fprintf(read_bandwidth_file, "%lf", read_bandwidth);
        aggregate_read_bandwidth += read_bandwidth;

        free(arr);
    }

    double average_read_bandwidth = aggregate_read_bandwidth / NUM_ITERATIONS;
    printf("Average read bandwidth (over %d iterations): %.3lf\n", NUM_ITERATIONS, average_read_bandwidth);
}

void measure_write_bandwidth() {
    FILE *write_bandwidth_file;
    write_bandwidth_file = fopen(WRITE_BANDWIDTH_RECORD_FILE, "w");
	if (!write_bandwidth_file) {
		printf("Error opening file %s\n", WRITE_BANDWIDTH_RECORD_FILE);
		exit(1);
	}

    double aggregate_write_bandwidth = 0;

    ll arr_size = ARR_SIZE * 1024 * 1024;
    ll arr_elem_cnt = (ll)(arr_size / sizeof(ll));
    ll *arr = (ll*)malloc(arr_size);

    for(int i = 0; i < NUM_ITERATIONS; i++) {
        CLEAR_CACHE;
        ll *arr = (ll*)malloc(arr_size);
        ll* start = arr;
        ll* end = arr + arr_elem_cnt;

        start_benchmark();
        do {
            *start = 1;
        } while(++start < end);
        end_benchmark();

        double write_cycles = report_cycles(1);
        // read_bandwidth calculcates bandwidth in MB/s
        double time_for_writes = TIME_PER_CYCLE * write_cycles;
        double write_bandwidth = ((ARR_SIZE * 1024 * 1024 * 8) / time_for_writes) * LINES_PER_CLOCK * BITS_PER_LINE * NUM_INTERFACES * BITS_TO_BYTES * BYTES_TO_MB;
        fprintf(write_bandwidth_file, "%lf", write_bandwidth);
        aggregate_write_bandwidth += write_bandwidth;

        free(arr);
    }

    double average_write_bandwidth = aggregate_write_bandwidth / NUM_ITERATIONS;
    printf("Average write bandwidth (over %d iterations): %.3lf\n", NUM_ITERATIONS, average_write_bandwidth);
}

int main(int argc, char const *argv[]) {
    measure_read_bandwidth();
    measure_write_bandwidth();
}
