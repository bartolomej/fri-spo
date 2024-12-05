#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define MAX_OVERALL_TRIES 1e9


long total_hits = 0;
long total_tries = 0;
pthread_mutex_t mutex;

typedef struct {
    long tries;
} WorkerParams;

void *worker(void *arg) {
    const WorkerParams *data = (WorkerParams *) arg;
    const long tries = data->tries;

    while (true) {
        long hits = 0;
        for (long i = 0; i < tries; i++) {
            const double x = ((double) rand() / RAND_MAX) * M_PI;
            const double y = ((double) rand() / RAND_MAX);
            if (y < sin(x)) {
                hits++;
            }
        }

        pthread_mutex_lock(&mutex);
        total_hits += hits;
        total_tries += tries;
        pthread_mutex_unlock(&mutex);

        if (total_tries >= MAX_OVERALL_TRIES) {
            break;
        }
    }

    return NULL;
}

int main(const int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <n_threads> <tries_per_thread>\n", argv[0]);
        return 1;
    }

    const int n_threads = atoi(argv[1]);
    const long tries_per_thread = atol(argv[2]);
    pthread_t threads[n_threads];
    WorkerParams thread_data[n_threads];

    pthread_mutex_init(&mutex, NULL);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < n_threads; i++) {
        thread_data[i].tries = tries_per_thread;
        pthread_create(&threads[i], NULL, worker, &thread_data[i]);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;

    pthread_mutex_destroy(&mutex);

    printf("total_hits = %ld, total_tries=%ld\n", total_hits, total_tries);
    double integral = ((double) total_hits / (double)total_tries) * M_PI;
    printf("Approx result: %.10f\n", integral);
    printf("Execution time: %.6fs\n", elapsed_time);

    return 0;
}
