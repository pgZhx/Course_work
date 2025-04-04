#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parallel.h"

#define N 512

float A[N][N], B[N][N], C[N][N];

typedef struct {
    int start;
    int end;
} MatrixArgs;
void* matrix_multiply_func(void* args) {
    MatrixArgs* margs = (MatrixArgs*)args;
    for (int i = margs->start; i < margs->end; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    return NULL; // 不需要释放内存，因为在主线程中管理
}

double get_wall_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main() {
    int num_threads = 8;

    // 初始化矩阵
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j] = 0;
        }
    }

    double start_time = get_wall_time();
    printf("Size: %d,Threads: %d, calculating...\n",N,num_threads);
    // 调用 parallel_for
    for (int t = 0; t < num_threads; t++) {
        MatrixArgs* args = (MatrixArgs*)malloc(sizeof(MatrixArgs));
        args->start = t * (N / num_threads);
        args->end = (t == num_threads - 1) ? N : (t + 1) * (N / num_threads);
        parallel_for(args->start, args->end, 1, matrix_multiply_func, args, num_threads);
    }

    double end_time = get_wall_time();
    printf("Matrix multiplication took %.3lf seconds\n", end_time - start_time);

    return 0;
}
