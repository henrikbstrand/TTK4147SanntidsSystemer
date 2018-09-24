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


void* respond(void* args){
	set_cpu(1);
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