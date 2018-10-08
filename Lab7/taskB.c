#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include <native/sem.h>


struct param{
	int id;
	RT_SEM *sem;	
};

struct semaphores{
	RT_SEM broadcast;
	RT_SEM sem;
};

void busy_wait_us(unsigned long delay){
	for(; delay > 0; delay--){
		rt_timer_spin(1000);	
	}
}

void low(struct semaphores *sems){
	rt_printf("low waiting for start\n");
	rt_sem_p(&sems->broadcast, TM_INFINITE);
	rt_printf("low starting\n");

	rt_sem_p(&sems->sem, TM_INFINITE);
	rt_printf("low locked sem\n");
	busy_wait_us(3000);
	rt_printf("low done\n");
	rt_sem_v(&sems->sem);
}

void medium(struct semaphores *sems){
	rt_printf("medium waiting for start\n");
	rt_sem_p(&sems->broadcast, TM_INFINITE);
	rt_printf("medium starting\n");

	rt_task_sleep(1000000);
	busy_wait_us(5000);
	rt_printf("medium done\n");
}

void high(struct semaphores *sems){
	rt_printf("high waiting for start\n");
	rt_sem_p(&sems->broadcast, TM_INFINITE);
	rt_printf("high starting\n");

	rt_task_sleep(2000000);
	rt_printf("high locking\n");
	rt_sem_p(&sems->sem, TM_INFINITE);	
	busy_wait_us(2000);
	rt_sem_v(&sems->sem);
	rt_printf("high done\n");
}



int main(){
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT | MCL_FUTURE);


	struct semaphores sems;

	rt_sem_create(&sems.broadcast, "sem",0, S_PRIO);
	rt_sem_create(&sems.sem, "sem",0, S_PRIO);

	RT_TASK taskLow;
	rt_task_create(&taskLow, "low", 0 ,50, T_CPU(1));
	rt_task_start(&taskLow, low, &sems);

	RT_TASK taskMedium;
	rt_task_create(&taskMedium, "medium", 0, 52, T_CPU(1));
	rt_task_start(&taskMedium, medium, &sems);

	RT_TASK taskHigh;
	rt_task_create(&taskHigh, "high", 0, 55, T_CPU(1));
	rt_task_start(&taskHigh, high, &sems);

	rt_task_sleep(100000000);


	rt_sem_broadcast(&sems.broadcast);

	rt_task_sleep(100000000);

	//rt_sem_delete(&sem);

	return 1;
}
