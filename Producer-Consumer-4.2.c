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
pthread_spinlock_t lock;

void *producer(void *param) {
    struct ThreadData *data = (struct ThreadData *)param;
    int id = data->thread_id;

    while (1) {
        pthread_spin_lock(&lock);

        if (next_produced_item >= data->upper_limit) {
            pthread_spin_unlock(&lock);
            break;
        }

        buffer[next_produced_item % BUFFER_SIZE] = next_produced_item;
        printf("%d, %d\n", next_produced_item, id);
        next_produced_item++;

        pthread_spin_unlock(&lock);
    }
    pthread_exit(NULL);
}

void *consumer(void *param) {
    struct ThreadData *data = (struct ThreadData *)param;
    int id = data->thread_id;

    while (1) {
        pthread_spin_lock(&lock);

        if (next_consumed_item >= data->upper_limit) {
            pthread_spin_unlock(&lock);
            break;
        }

        printf("%d, %d\n", buffer[next_consumed_item % BUFFER_SIZE], id);
        next_consumed_item++;

        pthread_spin_unlock(&lock);
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

    pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);

    pthread_t tid_producer[num_producers];
    pthread_t tid_consumer[num_consumers];

    struct ThreadData producer_data[num_producers];
    struct ThreadData consumer_data[num_consumers];

    for (int i = 0; i < num_producers; ++i) {
        producer_data[i].thread_id = i + 1;
        producer_data[i].upper_limit = upper_limit;
        pthread_create(&tid_producer[i], NULL, producer, &producer_data[i]);
    }

    for (int i = 0; i < num_consumers; ++i) {
        consumer_data[i].thread_id = i + 1;
        consumer_data[i].upper_limit = upper_limit;
        pthread_create(&tid_consumer[i], NULL, consumer, &consumer_data[i]);
    }

    for (int i = 0; i < num_producers; ++i) {
        pthread_join(tid_producer[i], NULL);
    }

    for (int i = 0; i < num_consumers; ++i) {
        pthread_join(tid_consumer[i], NULL);
    }

    pthread_spin_destroy(&lock);

    return 0;
}
