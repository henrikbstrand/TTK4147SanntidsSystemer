#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>



// Call these at the start of main

void print(){
	long period = 1*1000*1000*1000;
	rt_task_set_periodic(NULL, TM_NOW, period);
	while(1){
		rt_printf("running\n");
		rt_task_wait_period(NULL);
	}
	
}
/*
int main(){
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT | MCL_FUTURE);

	RT_TASK PeriodicPrinter;

	rt_task_create(&PeriodicPrinter, "PeriodicPrinter", 0, 0, T_CPU(1));
	rt_task_start(&PeriodicPrinter, print, NULL);
	
	while(1){}
	//rt_task_join(&PeriodicPrinter);

}*/
