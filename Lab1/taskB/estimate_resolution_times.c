#include <sys/times.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>

int main(){

    int ns_max = 50;
        int histogram[ns_max];
        memset(histogram, 0, sizeof(int)*ns_max);

        for(int i = 0; i < 10*1000*1000; i++){
            
            struct tms t1;
            times(&t1);
            struct tms t2;
            times(&t2);
            
            int ns = (t2.tms_stime /*+ t2.tms_utime*/ - (t1.tms_stime /*+ t1.tms_utime*/));
            
            if(ns >= 0 && ns < ns_max){
                histogram[ns]++;
            }
        }

        for(int i = 0; i < ns_max; i++){
            printf("%d\n", histogram[i]);
        }

}