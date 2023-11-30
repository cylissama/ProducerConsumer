/* 
 * lab4_task3.c
 */


#include<pthread.h>
#include<stdio.h>
#include<semaphore.h>

int n;     
sem_t psem, csem; 

void *producer(void *param); 
void *consumer(void *param); 

int main(int argc, char *argv[])
{
  pthread_t tid_producer, tid_consumer;    /* thread identifier */
  pthread_attr_t attr_producer, attr_consumer;  /* set of thread attributes */
  int i, policy;
  pthread_attr_t attr; 		/* set of attributes for the thread */  
  clock_t start, end; 
  double elapse_time; 

  start = clock(); 

  sem_init(&psem, 0, 1); 
  sem_init(&csem, 0, 0); 

  /* get default attribute */
  pthread_attr_init(&attr_producer);
  pthread_attr_init(&attr_consumer);

  /*create threads */
  pthread_create(&tid_producer, &attr_producer, producer, NULL);
  pthread_create(&tid_consumer, &attr_consumer, consumer, NULL);

  /* wait for producer and consumer threads to exit */
  pthread_join(tid_producer, NULL);
  pthread_join(tid_consumer, NULL);

  end = clock(); 
  elapse_time = ((double) (end - start))/ CLOCKS_PER_SEC;
  printf("main thread elapse_time %f\n", elapse_time); 
}

void *producer(void *param)
{
  int i;
  printf("producer starts ...\n"); 

  for (i=0; i<1e8; i++)
  {  
     sem_wait(&psem);
     n++; 
     sem_post(&csem);
  }

  printf("producer exits.\n"); 
  pthread_exit(0);
}

/* ''consume'' and print value n */
void *consumer(void *param)
{
  int i;
  printf("consumer starts ...\n"); 

  for (i=0; i<1e6; i++)
  { 
     sem_wait(&csem);
     sem_post(&psem);
  }

  printf("consumer exits. \n"); 
  pthread_exit(0);
}