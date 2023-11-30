#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/errno.h>

//int *buffer;
int prodNum, conNum, buffer_size, limit;
sem_t *empty;
sem_t *full;
int count = 0;
int buffer[5];
pthread_mutex_t mutex;

void *producer(void *param)
{  
    while(1) {
        //make 50 limit
        int item = rand() % 50;
        //printf("%d",count);
        //printf(" %d ",sem_wait(&empty));
        errno=0;
        int check = sem_wait(&empty);
        //printf("prod check val = %d\n",check);
        if (check==0){
            printf("Full\n");
        } else if (check > 0){
            printf("yippe\n");
        } else if (check < 0) {
            printf("ERROR\n");
        } else {
            printf("BAD NEWS\n");
        }
        printf("sem_open() failed.  errno:%d\n", errno);

        pthread_mutex_lock(&mutex);
        buffer[count] = item;
        count++;
        printf("Producer %d: Insert Item %d at buffer[%d]\n", *((int *)param),item,count);
        //in = (in+1)%buffer_size;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);

        //return NULL;
    }
}
void *consumer(void *param)
{   
    while(1) {
        int item;
        int check = sem_wait(&full);
        //printf("con check val = %d\n",check);
        if (check == 0){
            printf("Full\n");
        } else if (check > 0){
            printf("yippe\n");
        } else {
            printf("ERROR");
        }
        pthread_mutex_lock(&mutex);
        item = buffer[count - 1];
        count--;
        //out = (out+1)%buffer_size;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        printf("Consumer %d: Remove Item %d from buffer [%d]\n",*((int *)param),item, count);

        sleep(2);
        //return NULL;
    }
}

struct data {
    int one;
    char two;
    int a[];
};

int main(int argc, char *argv[])
{   
    srand(time(NULL));

    if (argc == 5) {
        //printf("worked %d %d %d %d\n",atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
        buffer_size = atoi(argv[1]);
        //buffer = (int *)malloc(buffer_size * sizeof(int)); // Allocate memory for the buffer;
        prodNum = atoi(argv[2]);
        conNum = atoi(argv[3]);
        limit = atoi(argv[4]);
    } else {
        printf("Buffer Size: ");
        scanf("%d", &buffer_size);
        //buffer = (int *)malloc(buffer_size * sizeof(int)); // Allocate memory for the buffer

        printf("Num of Producers: ");
        scanf("%d", &prodNum);

        printf("Num of Consumers: ");
        scanf("%d", &conNum);

        //currently does nothing
        printf("Upper Limit: ");
        scanf("%d", &limit);
        printf("\n");
    }
    pthread_t pro[prodNum],con[conNum];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,10);
    sem_init(&full,0,0);

    struct data *s1 = (struct data *) malloc(sizeof(struct data));
    s1->one = 1;
    s1->two = '2';

    s1->a;

    int higher;
    if(prodNum > conNum) {
        higher = prodNum;
    } else {
        higher = conNum;
    }

    for(int i = 0; i < higher; i++) {
        s1->a[i] = i;
        //printf("%d\n",i);
    }

    for(int i = 0; i < prodNum; i++) {
        //printf("Creating producer #%d\n",i);
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&s1->a[i]);
    }
    for(int j = 0; j < conNum; j++) {
        //printf("Creating consumer #%d\n",j);
        pthread_create(&con[j], NULL, (void *)consumer, (void *)&s1->a[j]);
    }
    
    for(int i = 0; i < prodNum; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int j = 0; j < conNum; j++) {
        pthread_join(con[j], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
    
}