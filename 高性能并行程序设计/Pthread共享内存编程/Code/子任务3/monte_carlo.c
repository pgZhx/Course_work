#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_POINTS 50000000
#define NUM_THREADS 8

// 全局变量
int points_inside_curve = 0;
int total_points = 0;
pthread_mutex_t mutex;

// 线程函数
void *monte_carlo_thread(void *arg) {
    int thread_id = *((int *)arg);
    int points_inside = 0;
    int points_generated = 0;
    for (int i = 0; i < NUM_POINTS / NUM_THREADS; i++) {
        double x = rand() / (double)RAND_MAX;
        double y = rand() / (double)RAND_MAX;
        //printf("(%.2f,%.2f)\n",x,y);
        if (y <= x * x) {
            points_inside++;
        }
        points_generated++;
    }
    // 锁定互斥锁以更新全局计数器
    pthread_mutex_lock(&mutex);
    points_inside_curve += points_inside;
    total_points += points_generated;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    srand((unsigned int)time(NULL)); // 初始化随机数生成器
    // 初始化互斥锁
    pthread_mutex_init(&mutex, NULL);
    // 创建线程
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, monte_carlo_thread, (void *)&thread_ids[i]);
    }
    // 等待所有线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    // 计算面积
    double area = (double)points_inside_curve / total_points;
    // 输出结果
    printf("Estimated area under the curve y = x^2 from 0 to 1: %.12lf\n", area);
    // 销毁互斥锁
    pthread_mutex_destroy(&mutex);
    return 0;
}