#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "parallel.h"

typedef struct {
    int start;
    int end;
    int increment;
    void*(*functor)(void*);
    void* arg;
} ThreadArgs;

void* thread_func(void* args) {
    ThreadArgs* t = (ThreadArgs*)args;
    for (int i = t->start; i < t->end; i += t->increment) {
        t->functor(t->arg); // 直接调用 functor
    }
    free(t); // 释放分配的参数
    return NULL;
}

void parallel_for(int start, int end, int increment, void*(*functor)(void*), void* arg, int num_threads) {
    if (num_threads <= 0) {
        fprintf(stderr, "Invalid number of threads: %d\n", num_threads);
        exit(EXIT_FAILURE);
    }

    pthread_t threads[num_threads];
    int range = (end - start) / num_threads;

    for (int i = 0; i < num_threads; i++) {
        ThreadArgs* thread_args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        thread_args->start = start + i * range;
        thread_args->end = (i == num_threads - 1) ? end : (start + (i + 1) * range);
        thread_args->increment = increment;
        thread_args->functor = functor;
        thread_args->arg = arg;

        if (pthread_create(&threads[i], NULL, thread_func, thread_args) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}
