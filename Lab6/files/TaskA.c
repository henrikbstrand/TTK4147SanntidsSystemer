#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>
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

void* disturb(void* args){
	set_cpu(1);
	while(1){
		asm volatile("" ::: "memory");
	}
}

void respond(void * argPin){

	int pin = (*(int*)argPin);

	//Start timer
	unsigned long duration = 10*1000*1000;  // 10 second timeout
	unsigned long endTime = rt_timer_read() + duration;

	while(1){
		
		//Poll pin
		int value = 1;
		value = io_read(pin);
		if(value == 0){
			io_write(pin, 0);
			rt_timer_spin(5000);
			io_write(pin, 1);
		}
		if(rt_timer_read() > endTime){
			rt_printf("Time expired\n");
			rt_task_delete(NULL);
		}

		if(rt_task_yield()){
			rt_printf("Task failed to yield\n");
			rt_task_delete(NULL);
		}
	}
}




int main(){
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	
	io_init();

	int pinA = 1;
	RT_TASK RespondTaskA;
	rt_task_create(&RespondTaskA, "ResponsA", 0, 0, T_CPU(1));
	rt_task_start(&RespondTaskA, respond, &pinA);

	int pinB = 2;
	RT_TASK RespondTaskB;
	rt_task_create(&RespondTaskB, "ResponsB", 0, 0, T_CPU(1));
	rt_task_start(&RespondTaskB, respond, &pinB); 

	int pinC = 3;
	RT_TASK RespondTaskC;
	rt_task_create(&RespondTaskC, "ResponsC", 0, 0, T_CPU(1));
	rt_task_start(&RespondTaskC, respond, &pinC);

	pthread_t disturbanceThreads[10];
	for (int i = 0; i < 10; i++){
		pthread_create(&disturbanceThreads[i], NULL, disturb, NULL);
	}
	
	while(1){}
	return 1;
}
