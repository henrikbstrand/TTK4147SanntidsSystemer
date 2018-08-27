#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <x86intrin.h>
#include <stdio.h>
#include <string.h>

int main() {
    double n = 10*1000*1000;
    
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for(int i = 0; i < n; i++){
        // read timer
        __rdtsc();
    }

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    printf("\ntime __rdtsc() : %f", (end.tv_nsec - start.tv_nsec)/n);

    ////////////////////////////////////////////////


    clock_gettime(CLOCK_MONOTONIC, &start);

    for(int i = 0; i < n; i++){
        // read timer
        struct timespec buff;
        clock_gettime(CLOCK_MONOTONIC, &buff);
    }

   
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    printf("\ntime clock_gettime() : %f", (end.tv_nsec - start.tv_nsec)/n);

    ////////////////////////////////////////////////////

  
    clock_gettime(CLOCK_MONOTONIC, &start);

    for(int i = 0; i < n; i++){
        // read timer
        struct tms buf;
        times(&buf);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    printf("\ntime times(): %f \n", (end.tv_nsec - start.tv_nsec)/n);

    return 1;
}