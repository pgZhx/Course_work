#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h> // 包含gettimeofday
#define MAX 2048 // 矩阵的最大尺寸
int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX]; // 定义全局矩阵
int M, N, K; // M*N 和 N*K 的矩阵维度
// 线程数量
int num_threads = 8;
// 线程参数结构体
typedef struct {
    int thread_id;
    int start_row;
    int end_row;
} thread_data_t;
// 线程执行函数
void *multiply_matrix(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int i, j, k;
    // 计算属于该线程的部分矩阵乘法
    for (i = data->start_row; i < data->end_row; i++) {
        for (j = 0; j < K; j++) {
            C[i][j] = 0;
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}
int main() {
    // 输入矩阵维度
    int size = 2048;
    M = N = K = size;
    // 初始化矩阵A和B
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10; // 随机生成矩阵元素
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < K; j++) {
            B[i][j] = rand() % 10;
        }
    }
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];
    int rows_per_thread = M / num_threads; // 每个线程负责的行数
    int remaining_rows = M % num_threads; // 处理剩余的行
    struct timeval start, end;
    gettimeofday(&start, NULL); // 记录开始时间
    // 创建线程并分配任务
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].start_row = i * rows_per_thread;
        if (i == num_threads - 1) {
            thread_data[i].end_row = (i + 1) * rows_per_thread + remaining_rows;
        } else {
            thread_data[i].end_row = (i + 1) * rows_per_thread;
        }
        pthread_create(&threads[i], NULL, multiply_matrix, (void *)&thread_data[i]);
    }
    // 等待所有线程完成
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end, NULL); // 记录结束时间
    // 计算时间差（单位：微秒）
    double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0; // 秒转毫秒
    elapsed_time += (end.tv_usec - start.tv_usec) / 1000.0; // 微秒转毫秒
    // 打印计算时间
    printf("Size: %d, nums of thread: %d , Time taken for matrix multiplication: %lf ms\n",size,num_threads, elapsed_time);
    return 0;
}
