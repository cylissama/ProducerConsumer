#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

sem_t semEmpty;
sem_t semFull;
pthread_mutex_t mutexBuffer;

int* buffer; // Dynamically allocated buffer
int bufferSize; // Buffer size
int count = 0;
int limit = 0;
int number = 0;

void* producer(void* args) {
    while (1) {
        if(number >= limit)
            printf("End");
        int x = number++; // Generate random number
        sleep(1);

        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        buffer[count] = x;
        count++;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
}

void* consumer(void* arg) {
    int consumer_id = *((int*)arg); // Extract unique consumer ID

    while (1) {
        int y;

        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        y = buffer[count - 1];
        count--;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);

        printf("%d, %d\n", y, consumer_id);
        sleep(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: %s <buffer_size> <num_producers> <num_consumers> <limit>\n", argv[0]);
        return 1;
    }

    bufferSize = atoi(argv[1]); // Get buffer size from command line
    buffer = (int*)malloc(bufferSize * sizeof(int)); // Allocate memory for buffer

    int numProducers = atoi(argv[2]);
    int numConsumers = atoi(argv[3]);
    limit = atoi(argv[4]);

    srand(time(NULL));
    pthread_t th[numProducers + numConsumers];
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, bufferSize);
    sem_init(&semFull, 0, 0);

    int i;
    for (i = 0; i < numProducers; i++) {
        if (pthread_create(&th[i], NULL, &producer, NULL) != 0) {
            perror("Failed to create producer thread");
        }
    }

    int consumer_ids[numConsumers];
    for (int j = 0; j < numConsumers; j++) {
        consumer_ids[j] = j; // Assign unique IDs to consumers
        if (pthread_create(&th[i + j], NULL, &consumer, &consumer_ids[j]) != 0) {
            perror("Failed to create consumer thread");
        }
    }

    for (i = 0; i < numProducers + numConsumers; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    free(buffer);

    return 0;
}
