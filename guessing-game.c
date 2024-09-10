#include "guessing-game.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>

#define MIN_NUMBER 1
#define MAX_NUMBER 10
#define CORRECT_NUMBER 5

static pthread_mutex_t finished_mutex;
static pthread_cond_t finished_cond;
static bool finished = false;
static sem_t *s;
static int num_threads;

void *thread_func(void *arg) {
    const int thread_no = *(int *) arg;

    while (true) {
        sem_wait(&s[thread_no]);

        pthread_mutex_lock(&finished_mutex);
        if (finished) {
            pthread_mutex_unlock(&finished_mutex);
            sem_post(&s[(thread_no + 1) % num_threads]);
            free(arg);
            int *result = malloc(sizeof(int));
            *result = 0;
            return result;
        }

        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        srand(ts.tv_nsec + thread_no + pthread_self());
        const int guess = rand() % (MAX_NUMBER - MIN_NUMBER + 1) + MIN_NUMBER;
        printf("Thread %d guessed %d\n", thread_no, guess);

        if (guess == CORRECT_NUMBER) {
            finished = true;
            pthread_cond_broadcast(&finished_cond);
            pthread_mutex_unlock(&finished_mutex);
            sem_post(&s[(thread_no + 1) % num_threads]);
            free(arg);
            int *result = malloc(sizeof(int));
            *result = 1;
            return result;
        }
        pthread_mutex_unlock(&finished_mutex);

        sleep(1);
        sem_post(&s[(thread_no + 1) % num_threads]);
    }
}

void start_guessing_game(const int no_threads) {
    num_threads = no_threads;
    pthread_t threads[num_threads];
    s = malloc(num_threads * sizeof(sem_t));

    sem_init(&s[0], 0, 1);
    for (int i = 1; i < num_threads; i++) {
        sem_init(&s[i], 0, 0);
    }

    pthread_mutex_init(&finished_mutex, NULL);
    pthread_cond_init(&finished_cond, NULL);

    for (int i = 0; i < num_threads; i++) {
        int *j = malloc(sizeof(int));
        if (j == NULL) {
            fprintf(stderr, "Memory allocation failed for thread %d\n", i);
            exit(EXIT_FAILURE);
        }
        *j = i;

        const int ret = pthread_create(&threads[i], NULL, thread_func, j);
        if (ret != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            free(j);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        int *ret_val;
        pthread_join(threads[i], (void**)&ret_val);
        if (*ret_val == 1) {
            printf("Thread %d won!\n", i);
        }
        free(ret_val);
    }

    for (int i = 0; i < num_threads; i++) {
        sem_destroy(&s[i]);
    }

    free(s);
    pthread_mutex_destroy(&finished_mutex);
    pthread_cond_destroy(&finished_cond);
}
