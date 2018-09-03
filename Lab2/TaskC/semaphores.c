#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

sem_t var_sem;

// Note the argument and return types: void*
void* fn(void* args){
    long copy = 0;
    

    for (int i = 0; i < 50000000; i++) {
        copy++;
        sem_wait(&var_sem);
        (*(long*)args)++;
        sem_post(&var_sem);
    }
    printf("copy:\t %lu\n", copy);
    return NULL;
}

int main(){
    long var = 0;
    sem_init(&var_sem, 1, 1);

    pthread_t threadHandle;
    pthread_create(&threadHandle, NULL, fn, &var);    
    
    pthread_t threadHandle1;
    pthread_create(&threadHandle1, NULL, fn, &var);    
    
    pthread_join(threadHandle, NULL);
    pthread_join(threadHandle1, NULL);

    printf("var:\t %lu\n", var);
}
