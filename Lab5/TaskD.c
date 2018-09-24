#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>

#include "io.h"

/// Assigning CPU core ///

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

/// 'struct timespec' functions ///

struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
    }
    while(nsec < 0){
        nsec += 1000000000;
        --sec;
    }
    return (struct timespec){sec, nsec};
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

void* respond(void* args){
	set_cpu(1);
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);

    struct timespec period = {.tv_sec = 0, .tv_nsec = 1*1000*1000};


    int pin = (*(int*)args);
    while(1){
        int value = 1;
        value = io_read(pin);
        if(value == 0){
            //printf("value = 0\n");
            io_write(pin, 0);
            usleep(5000);
            io_write(pin, 1);
        }
        // sleep
        waketime = timespec_add(waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    }
}

void* disturb(void* args){
    set_cpu(1);
    while(1){
        asm volatile("" ::: "memory");
    }
}


int main(){
    
    io_init();

    int pinA = 1;
    pthread_t threadHandle;
    pthread_create(&threadHandle, NULL, respond, &pinA);    
    
    int pinB = 2;
    pthread_t threadHandle1;
    pthread_create(&threadHandle1, NULL, respond, &pinB);    

    int pinC = 3;
    pthread_t threadHandle2;
    pthread_create(&threadHandle2, NULL, respond, &pinC);    
    
    
    pthread_t disturbanceThreads[10];
    for (int i = 0; i < 10; i++){
        pthread_create(&disturbanceThreads[i], NULL, disturb, NULL);    

    }

    while(1){}
}