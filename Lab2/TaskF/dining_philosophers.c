#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define num_phil 5

sem_t dine_sem ;
pthread_mutex_t forks[num_phil];

void think(){
    for (int i = 0; i < 50000000; i++) {
        i++;
        i--;
    }
}

void eat(int id){
    sem_wait(&dine_sem);

    if(id == num_phil-1){
        pthread_mutex_lock(&forks[num_phil-1]);
        think();
        pthread_mutex_lock(&forks[0]);
        printf("I %d, am eating\n", id);
        pthread_mutex_unlock(&forks[num_phil-1]);
        pthread_mutex_unlock(&forks[0]);
    }
    else{
        pthread_mutex_lock(&forks[id]);
        think();
        pthread_mutex_lock(&forks[id+1]);
        printf("I %d, am eating\n", id);
        pthread_mutex_unlock(&forks[id]);
        pthread_mutex_unlock(&forks[id+1]);
    }

    sem_post(&dine_sem);
}

void * dine(void* args){
    int id = (*(long*)args);
    while(1){
        think();
        eat(id);
    }
}

int main(){
    
    sem_init(&dine_sem, 1, num_phil-1);

    pthread_t philosophers[num_phil];

    for (int i = 0; i < num_phil; i++) {
        pthread_mutex_init(&forks[i], NULL);
        printf("mutex created\n");
    }

    for (int i = 0; i < num_phil; i++) {
        pthread_create(&philosophers[i], NULL, dine, &i);
        printf("thread spawned\n");
    }    

    pthread_join(philosophers[0], NULL);
}
