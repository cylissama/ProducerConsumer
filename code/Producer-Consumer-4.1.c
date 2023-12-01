#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

struct ThreadData {
    int thread_id;
    int upper_limit;
};

int buffer[BUFFER_SIZE];
int next_produced_item = 0;
int next_consumed_item = 0;
sem_t mutex, full, empty;

void *producer(void *param) {
    struct ThreadData *data = (struct ThreadData *)param;
    int id = data->thread_id;

    while (1) {
        sem_wait(&empty);
        sem_wait(&mutex);

        if (next_produced_item >= data->upper_limit) {
            sem_post(&mutex);
            sem_post(&full);
            break;
        }

        buffer[next_produced_item % BUFFER_SIZE] = next_produced_item;
        next_produced_item++;

        sem_post(&mutex);
        sem_post(&full);
    }
    pthread_exit(NULL);
}

void *consumer(void *param) {
    struct ThreadData *data = (struct ThreadData *)param;
    int id = data->thread_id;

    while (1) {
        sem_wait(&full);
        sem_wait(&mutex);

        if (next_consumed_item >= data->upper_limit) {
            sem_post(&mutex);
            sem_post(&empty);
            break;
        }

        int item_to_consume = buffer[next_consumed_item % BUFFER_SIZE];
        printf("%d, %d\n", item_to_consume, id);
        next_consumed_item++;

        sem_post(&mutex);
        sem_post(&empty);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <buffer_size> <num_producers> <num_consumers> <upper_limit>\n", argv[0]);
        return 1;
    }

    int buffer_size = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);
    int upper_limit = atoi(argv[4]);

    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, buffer_size);

    pthread_t tid_producer[num_producers];
    pthread_t tid_consumer[num_consumers];

    struct ThreadData producer_data[num_producers];
    struct ThreadData consumer_data[num_consumers];

    for (int i = 0; i < num_producers; ++i) {
        producer_data[i].thread_id = i + 1; // Adjusting thread ID starting from 1
        producer_data[i].upper_limit = upper_limit;
        pthread_create(&tid_producer[i], NULL, producer, &producer_data[i]);
    }

    for (int i = 0; i < num_consumers; ++i) {
        consumer_data[i].thread_id = i + 1; // Adjusting thread ID starting from 1
        consumer_data[i].upper_limit = upper_limit;
        pthread_create(&tid_consumer[i], NULL, consumer, &consumer_data[i]);
    }

    for (int i = 0; i < num_producers; ++i) {
        pthread_join(tid_producer[i], NULL);
    }

    for (int i = 0; i < num_consumers; ++i) {
        pthread_join(tid_consumer[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}
