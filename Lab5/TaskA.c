#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>

#include "io.h"

void* respond(void* args){
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

    while(1){}
}