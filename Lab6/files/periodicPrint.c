#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>



// Call these at the start of main

void print(void * cookie){
	rt_task_set_periodic(&PeriodicPrinter, TM_NOW, 10*1000*1000);
	rt_printf("running\n");
}

int main(){
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT | MCL_FUTURE);

	RT_TASK PeriodicPrinter;

	rt_task_create(&PeriodicPrinter, "PeriodicPrinter", 0, 0, T_CPU(cupid));
	rt_task_start(&PeriodicPrinter, print(NULL), NULL);

}
