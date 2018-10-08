#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include <native/sem.h>
#include <native/mutex.h>
#include "taskE.h"


RT_MUTEX mutexA;
RT_MUTEX mutexB;
RT_SEM broadcast;

void busy_wait_usE(unsigned long delay){
	for(; delay > 0; delay--){
		rt_timer_spin(1000);	
	}
}

void lowE(){
	rt_printf("low waiting for start\n");
	rt_sem_p(&broadcast, TM_INFINITE);
	rt_printf("low starting\n");
	
	rt_task_set_priority(NULL, 60);
	rt_mutex_acquire(&mutexA, TM_INFINITE);
	
	rt_printf("low locked A\n");
	busy_wait_usE(3000);
	

	rt_mutex_acquire(&mutexB, TM_INFINITE);
	rt_printf("low locked B\n");
	busy_wait_usE(3000);
	
	rt_mutex_release(&mutexB);
	rt_mutex_release(&mutexA);	
	
	rt_task_set_priority(NULL, 50);	

	busy_wait_usE(1000);
	rt_printf("low done\n");
}


void highE(){
	rt_printf("high waiting for start\n");
	rt_sem_p(&broadcast, TM_INFINITE);	

	rt_printf("high Starting\n");
	rt_task_sleep(1000000);
	rt_mutex_acquire(&mutexB, TM_INFINITE);
	rt_printf("high locked B\n");
	busy_wait_usE(1000);
	rt_mutex_acquire(&mutexA, TM_INFINITE);
	rt_printf("high locked A\n");	

	busy_wait_usE(2000);
	rt_mutex_release(&mutexA);
	rt_mutex_release(&mutexB);	
	busy_wait_usE(1000);
	rt_printf("high done\n");
}



int taskE(){
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	
	rt_sem_create(&broadcast, "Broad",0, S_PRIO);

	rt_mutex_create(&mutexA, "mutexA");
	rt_mutex_create(&mutexB, "mutexB");

	RT_TASK taskLow;
	rt_task_create(&taskLow, "low", 0 ,50, T_CPU(1));
	rt_task_start(&taskLow, lowE, NULL);


	RT_TASK taskHigh;
	rt_task_create(&taskHigh, "high", 0, 55, T_CPU(1));
	rt_task_start(&taskHigh, highE, NULL);

	
	rt_task_sleep(100000000);

		
	rt_printf("\nBROADCAST\n");
	rt_sem_broadcast(&broadcast);

	while(1){rt_task_sleep(100000000);}

	return 1;
}
