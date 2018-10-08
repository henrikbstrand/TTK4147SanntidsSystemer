#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include <native/sem.h>
#include "taskA.h"


struct param{
	int id;
	RT_SEM *sem;	
};

void waiter(struct param *par){
	rt_sem_p(par->sem, TM_INFINITE);
	rt_printf("%d got SEM \n", par->id);
}



int taskA(){
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT | MCL_FUTURE);

	rt_task_shadow(NULL, "main", 99, T_CPU(1));

	RT_SEM sem;

	rt_sem_create(&sem, "sem",0, S_PRIO);

	struct param par1;
	struct param par2;

	par1.sem = &sem; 
	par2.sem = &sem;
	
	par1.id = 1;
	par2.id = 2;

	RT_TASK TaskA;
	rt_task_create(&TaskA, "A", 0 ,51, T_CPU(1));
	rt_task_start(&TaskA, waiter, &par1);

	RT_TASK TaskB;
	rt_task_create(&TaskB, "B", 0, 52, T_CPU(1));
	rt_task_start(&TaskB, waiter, &par2);

	rt_task_sleep(100000000);

	rt_printf("brodcastings sem \n");
	rt_sem_broadcast(&sem);

	rt_task_sleep(100000000);

	rt_sem_delete(&sem);

	return 1;
}
