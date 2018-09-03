#include <pthread.h>
#include <stdio.h>

// Note the argument and return types: void*
void* fn(void* args){
    long copy = 0;
    

    for (int i = 0; i < 50000000; i++) {
        copy++;
        (*(long*)args)++;
    }
    printf("copy:\t %lu\n", copy);
    return NULL;
}

int main(){
    long var = 0;

    pthread_t threadHandle;
    pthread_create(&threadHandle, NULL, fn, &var);    
    
    pthread_t threadHandle1;
    pthread_create(&threadHandle1, NULL, fn, &var);    
    
    pthread_join(threadHandle, NULL);
    pthread_join(threadHandle1, NULL);

    printf("var:\t %lu\n", var);
}
