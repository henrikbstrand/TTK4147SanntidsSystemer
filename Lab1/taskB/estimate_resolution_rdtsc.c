#include <x86intrin.h>
#include <stdio.h>
#include <string.h>

int main(){

    int ns_max = 50;
        int histogram[ns_max];
        memset(histogram, 0, sizeof(int)*ns_max);

        for(int i = 0; i < 10*1000*1000; i++){
            
            double t1 = __rdtsc()/2.66;
            double t2 = __rdtsc()/2.66;
            
            int ns = (t2 - t1);
            
            if(ns >= 0 && ns < ns_max){
                histogram[ns]++;
            }
        }

        for(int i = 0; i < ns_max; i++){
            printf("%d\n", histogram[i]);
        }

}