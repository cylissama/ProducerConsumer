#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Define shared variables, semaphores, and data structures
int buffer_size;
int upper_limit;
int *buffer;
int producer_counter = 0;
int consumer_counter = 0;

sem_t mutex; // Semaphore for mutual exclusion
sem_t empty; // Semaphore to track empty slots in the buffer
sem_t full;  // Semaphore to track filled slots in the buffer

struct ThreadData {
    int id;
};

// Producer function
void *producer(void *param) {
    struct ThreadData *data = (struct ThreadData *)param;

    while (1) {
        // Generate and produce data
        int produced_data = producer_counter;
        producer_counter++;
        
        // Wait for an empty slot in the buffer
        sem_wait(&empty);
        sem_wait(&mutex);

        // Place the data into the buffer
        buffer[consumer_counter % buffer_size] = produced_data;

        // Print the produced data and consumer ID
        printf("%d, %d\n", produced_data, data->id);
        
        consumer_counter++;
        
        sem_post(&mutex);
        sem_post(&full);

        if (produced_data >= upper_limit) {
            break;
        }
    }

    pthread_exit(0);
}

// Consumer function
void *consumer(void *param) {
    struct ThreadData *data = (struct ThreadData *)param;

    while (1) {
        // Wait for a filled slot in the buffer
        sem_wait(&full);
        sem_wait(&mutex);

        // Consume data from the buffer
        int consumed_data = buffer[consumer_counter % buffer_size];

        // Print the consumed data and consumer ID
        printf("%d, %d\n", consumed_data, data->id);
        
        consumer_counter++;

        sem_post(&mutex);
        sem_post(&empty);

        if (consumed_data >= upper_limit) {
            break;
        }
    }

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <buffer_size> <num_producers> <num_consumers> <upper_limit>\n", argv[0]);
        return 1;
    }

    buffer_size = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);
    upper_limit = atoi(argv[4]);

    buffer = (int *)malloc(buffer_size * sizeof(int));
    struct ThreadData *producer_data = (struct ThreadData *)malloc(num_producers * sizeof(struct ThreadData));
    struct ThreadData *consumer_data = (struct ThreadData *)malloc(num_consumers * sizeof(struct ThreadData));

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);

    pthread_t producer_threads[num_producers];
    pthread_t consumer_threads[num_consumers];

    // Create producer threads
    for (int i = 0; i < num_producers; i++) {
        producer_data[i].id = i;
        pthread_create(&producer_threads[i], NULL, producer, &producer_data[i]);
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; i++) {
        consumer_data[i].id = i;
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_data[i]);
    }

    // Join producer threads
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    // Clean up
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(buffer);
    free(producer_data);
    free(consumer_data);

    return 0;
}
