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
#include "taskC.h"



RT_SEM broadcast;
RT_MUTEX mutex;

void busy_wait_usM(unsigned long delay){
	for(; delay > 0; delay--){
		rt_timer_spin(1000);	
	}
}

void lowM(){
	rt_printf("low waiting for start\n");
	rt_sem_p(&broadcast, TM_INFINITE);
	rt_printf("low starting\n");

	rt_mutex_acquire(&mutex, TM_INFINITE);
	rt_printf("low locked sem\n");
	busy_wait_usM(3000);
	rt_printf("low unlocking\n");
	rt_printf("low done\n");
	rt_mutex_release(&mutex);
	
}

void mediumM(){
	rt_printf("medium waiting for start\n");
	rt_sem_p(&broadcast, TM_INFINITE);
	rt_printf("medium starting\n");

	rt_task_sleep(1000000);
	busy_wait_usM(5000);
	rt_printf("medium done\n");
}

void highM(){
	rt_printf("high waiting for start\n");
	rt_sem_p(&broadcast, TM_INFINITE);
	rt_printf("high starting\n");

	rt_task_sleep(2000000);
	rt_mutex_acquire(&mutex, TM_INFINITE);
	rt_printf("high locking\n");	
	busy_wait_usM(2000);
	rt_printf("high done\n");
	rt_mutex_release(&mutex);
}



int taskC(){
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT | MCL_FUTURE);

	rt_sem_create(&broadcast, "Broad",0, S_PRIO);
	rt_mutex_create(&mutex, "mutex");

	RT_TASK taskLow;
	rt_task_create(&taskLow, "low", 0 ,50, T_CPU(1));
	rt_task_start(&taskLow, lowM, NULL);

	RT_TASK taskMedium;
	rt_task_create(&taskMedium, "medium", 0, 52, T_CPU(1));
	rt_task_start(&taskMedium, mediumM, NULL);

	RT_TASK taskHigh;
	rt_task_create(&taskHigh, "high", 0, 55, T_CPU(1));
	rt_task_start(&taskHigh, highM, NULL);

	rt_task_sleep(1000000000);

		
	rt_printf("\nBROADCAST\n");
	rt_sem_broadcast(&broadcast);

	rt_task_sleep(100000000);


	return 1;
}
