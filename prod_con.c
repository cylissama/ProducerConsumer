#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

/*
This program provides a possible solution for producer-consumer problem using mutex and semaphore.
I have used 5 producers and 5 consumers to demonstrate the solution. You can always play with these values.
*/
int prodNum, conNum, buffer_size, limit;

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[];
pthread_mutex_t mutex;

void *producer(void *param)
{   
    int item;
    item = rand() % 50; // Produce an random item
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    buffer[in] = item;
    //printf("in = %d item = %d\n",in,item);
    printf("Producer %d: Insert Item %d at buffer[%d]\n", *((int *)param),buffer[in],in);
    //in = (in+1)%buffer_size;
    in++;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}
void *consumer(void *param)
{   
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    int item = buffer[out];
    //printf("out = %d item = %d\n",out,item);
    printf("Consumer %d: Remove Item %d from buffer [%d]\n",*((int *)param),item, out);
    //out = (out+1)%buffer_size;
    out++;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
}

struct data {
    int one;
    char two;
    int a[];
};

int main()
{   
    printf("Buffer Size: ");
    scanf("%d", &buffer_size);
    buffer[buffer_size];

    printf("Num of Producers: ");
    scanf("%d", &prodNum);

    printf("Num of Consumers: ");
    scanf("%d", &conNum);

    printf("Upper Limit: ");
    scanf("%d", &limit);
    printf("\n");

    pthread_t pro[prodNum],con[conNum];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,buffer_size);
    sem_init(&full,0,0);

    struct data *s1 = (struct data *) malloc(sizeof(struct data));
    s1->one = 1;
    s1->two = '2';

    s1->a;
    for(int i = 0; i < limit; i++) {
        s1->a[i] = i;
        //printf("%d\n",i);
    }

    for(int i = 0; i < prodNum; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&s1->a[i]);
    }
    for(int i = 0; i < conNum; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&s1->a[i]);
    }

    for(int i = 0; i < prodNum; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < conNum; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
    
}