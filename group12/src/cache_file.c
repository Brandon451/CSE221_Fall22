#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <stdint.h>
#include <benchmark.h>
#include <unistd.h>

#define KB 1024L                     //L is defined to support uint64
#define MB (1024 * KB)
#define GB (1024 * MB)
#define BLOCK_SIZE (4 * KB)

#define CYCLE_TIME 1/(1.7 * 1000)

#define FLUSH_CACHE ({ \
    char* data = "F"; \
    int fd = open("sudo /proc/sys/vm/drop_caches", O_WRONLY); \
    write(fd, data, sizeof(char)); \
    close(fd); \
    })

double read_file(int fd, char* buffer, uint64_t file_size) {
    uint64_t bytes_read = 0;

    start_benchmark();
    while((bytes_read += read(fd, buffer, BLOCK_SIZE)) < file_size);
    end_benchmark();

    double read_time = report_cycles(1) * CYCLE_TIME;
    return read_time;
}

int main(int argc, char const *argv[]) {
    
    int fd;
    char *buffer = (char *) malloc(BLOCK_SIZE);
    
    const char * files[] = {
        "../data/4K.dat",
        "../data/8K.dat",
        "../data/16K.dat",
        "../data/32K.dat",
        "../data/64K.dat",
        "../data/128K.dat",
        "../data/256K.dat",
        "../data/512K.dat",
        "../data/1M.dat",
        "../data/2M.dat",
        "../data/4M.dat",
        "../data/8M.dat",
        "../data/16M.dat",
        "../data/32M.dat",
        "../data/64M.dat",
        "../data/128M.dat",
        "../data/256M.dat",
        "../data/512M.dat",
        "../data/1G.dat",
        "../data/2G.dat",
        "../data/4G.dat",
        "../data/8G.dat"
    };

    uint64_t file_sizes[] = {
	4*KB,
        8*KB,
        16*KB,
        32*KB,
        64*KB,
        128*KB,
        256*KB,
        512*KB,
        1*MB,
        2*MB,
        4*MB,
        8*MB,
        16*MB,
        32*MB,
        64*MB,
        128*MB,
        256*MB,
        512*MB,
        1*GB,
        2*GB,
        4*GB,
        8*GB,
    };


    for(int i = 0; i < 22; i++) {
        FLUSH_CACHE;

        fd = open(files[i], O_RDONLY);

        // Load in cache
        lseek(fd, 0, SEEK_SET);
        double time_taken = read_file(fd, buffer, file_sizes[i]);

        //second time read, if present time will be low.
        lseek(fd, 0, SEEK_SET);
        time_taken = read_file(fd, buffer, file_sizes[i]);

        printf("Time Taken in mili-Second read is  %lf\n",  time_taken);
    }
    close(fd);
    free(buffer);
    return 0;
}
