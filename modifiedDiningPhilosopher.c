#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sched.h>
#include <pthread_impl.h>

sem_t chopsticks[];
sem_t eatRice;
pthread_t waiter;
pthread_t philosophers[];
pthread_mutex_t mutex;

int *amount_of_rice_eaten;
int all_rice_portion = 0;
int *uneaten_days;
int day_number;
int riceQuantity = 0;
int NUM_OF_PHILOSOPHER;

pthread_attr_t tattr;
struct sched_param param;

void *philosopher(void *);
void eat(int);
void *serve();

int main(int argc, char *argv[]){
    char *a = argv[1];
    NUM_OF_PHILOSOPHER = atoi(a);
    int n[NUM_OF_PHILOSOPHER];
    philosophers[NUM_OF_PHILOSOPHER];
    chopsticks[NUM_OF_PHILOSOPHER];
    day_number = 0;
    amount_of_rice_eaten = malloc(sizeof(int) * NUM_OF_PHILOSOPHER);
    pthread_mutex_init(&mutex, NULL);
    for(int i=0; i<NUM_OF_PHILOSOPHER; i++){
        amount_of_rice_eaten[i] = 0;
    }
    sem_init(&eatRice,0,1);
    riceQuantity = ( rand() % (NUM_OF_PHILOSOPHER-1) ) + 1;
    all_rice_portion = riceQuantity;
    for(int i=0; i<NUM_OF_PHILOSOPHER; i++){
        sem_init(&chopsticks[i],0,1);
        n[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void *)&n[i]);
    }
    pthread_create(&waiter, NULL, serve, NULL);
    for(int i=0; i<NUM_OF_PHILOSOPHER; i++){
        pthread_join(philosophers[i], NULL);
    }
    pthread_join(waiter, NULL);
    
    printf("--------------------------\n");
    printf("Amount of rice served by waiter: %d\n",all_rice_portion);
    for(int i=0; i<NUM_OF_PHILOSOPHER; i++){
        printf("Eaten Rice By Philosopher-%d is: %d\n",i,amount_of_rice_eaten[i]);
    } 
    printf("------------------------------\nROUND2 with high priority of first philosopher\n");
    day_number = 0;
    riceQuantity = ( rand() % (NUM_OF_PHILOSOPHER-1) ) + 1;
    all_rice_portion = riceQuantity;
    sem_init(&eatRice,0,1);

    //THREAD PRIORITY OF FIRST PHILOSOPHER INCREASED
    //int newprio = 20;
    int ret;
    ret = pthread_attr_init (&tattr);
    ret = pthread_attr_getschedparam (&tattr, &param);
    int priority_max = sched_get_priority_max(SCHED_FIFO);
    //printf("MAX PRIORITY:%d\n",priority_max);
    //(param.sched_priority)++;
    param.sched_priority = priority_max;
    ret = pthread_attr_setschedparam (&tattr, &param);
     
    for(int i=0; i<NUM_OF_PHILOSOPHER; i++){
        sem_init(&chopsticks[i],0,1);
        n[i] = i;
        if(i == 0){
            ret = pthread_create (&philosophers[0], &tattr, philosopher, (void *)&n[i] );
        }
        else{
            pthread_create(&philosophers[i], NULL, philosopher, (void *)&n[i]);
        }
    }
    pthread_create(&waiter, NULL, serve, NULL);
    for(int i=0; i<NUM_OF_PHILOSOPHER; i++){
        pthread_join(philosophers[i], NULL);
    }
    pthread_join(waiter, NULL);
    printf("--------------------------\n");
    printf("Amount of rice served by waiter: %d\n",all_rice_portion);
    for(int i=0; i<NUM_OF_PHILOSOPHER; i++){
        printf("Eaten Rice By Philosopher-%d is: %d\n",i,amount_of_rice_eaten[i]);
    }
    return 0;
}

void *philosopher(void *n){
    int ph_num = *(int *)n;
    while(1){
    //printf("Philosopher {%d} is thinking\n",ph_num);
    //printf("Philosopher {%d} tries to pick left chopstick\n",ph_num);
    sem_wait(&chopsticks[ph_num]);
    //printf("Philosopher {%d} picks the left chopstick\n",ph_num);
    //printf("Philosopher {%d} tries to pick right chopstick\n",ph_num);
    sem_wait(&chopsticks[(ph_num+1)%NUM_OF_PHILOSOPHER]);
    //printf("Philosopher {%d} picks the right chopstick\n",ph_num);
    if(day_number == 1000){
        //printf("Rice tükendi ve day number:%d\n",day_number);
        sem_post(&chopsticks[(ph_num+1)%NUM_OF_PHILOSOPHER]);
        //printf("Philosopher {%d} leaves the right chopstick\n",ph_num);
        sem_post(&chopsticks[ph_num]);
        //printf("Philosopher {%d} leaves the left chopstick\n",ph_num);
        return NULL;
    }
    pthread_mutex_lock(&mutex);
    while(riceQuantity == 0){
        if(day_number == 1000){
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
    }
    sem_wait(&eatRice);
    eat(ph_num);
    amount_of_rice_eaten[ph_num] = amount_of_rice_eaten[ph_num] + 1;
    sem_post(&eatRice);
    pthread_mutex_unlock(&mutex);
    //printf("Philosopher {%d} has finished eating\n",ph_num);
    sem_post(&chopsticks[(ph_num+1)%NUM_OF_PHILOSOPHER]);
    //printf("Philosopher {%d} leaves the right chopstick\n",ph_num);
    sem_post(&chopsticks[ph_num]);
    //printf("Philosopher {%d} leaves the left chopstick\n",ph_num);
    sleep(0.1);
    }  
}

void eat(int num){
    //printf("Philosopher {%d} starts to eat\n",num);
    riceQuantity--;
    //printf("Rice Amount:%d\n",riceQuantity);
}

void *serve(){
    srand(time(NULL));
    for(day_number=0; day_number<1000; day_number++){
        sleep(0.01);
        if(riceQuantity == 0){
            riceQuantity = ( rand() % (NUM_OF_PHILOSOPHER-1) ) + 1;
            all_rice_portion = all_rice_portion + riceQuantity;
        }
    }
    printf("Finished");   
}
