#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// 全局变量用于存储方程的系数、判别式和根
double a, b, c;
double discriminant;
double root1, root2;
// 互斥锁和条件变量
pthread_mutex_t mutex;
pthread_cond_t cond;
int threads_completed = 0; // 完成的线程计数器
// 线程1：计算判别式
void *calculate_discriminant(void *arg) {
    pthread_mutex_lock(&mutex);
    discriminant = b * b - 4 * a * c;
    pthread_mutex_unlock(&mutex);
    // 通知主线程此线程已完成
    pthread_mutex_lock(&mutex);
    threads_completed++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}
// 线程2：计算根
void *calculate_roots(void *arg) {
    pthread_mutex_lock(&mutex);
    if (discriminant >= 0) {
        root1 = (-b + sqrt(discriminant)) / (2 * a);
        root2 = (-b - sqrt(discriminant)) / (2 * a);
    } else {
        root1 = root2 = 0; // 或者其他表示无实根的值
    }
    pthread_mutex_unlock(&mutex);
    // 通知主线程此线程已完成
    pthread_mutex_lock(&mutex);
    threads_completed++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main() {
    // 输入一元二次方程的系数
    printf("Enter coefficients a, b, and c (for equation ax^2 + bx + c = 0): ");
    scanf("%lf %lf %lf", &a, &b, &c);
    // 初始化互斥锁和条件变量
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t thread1, thread2;
    // 创建线程计算判别式
    pthread_create(&thread1, NULL, calculate_discriminant, NULL);
    // 创建线程计算根
    pthread_create(&thread2, NULL, calculate_roots, NULL);
    // 等待两个线程完成
    pthread_mutex_lock(&mutex);
    while (threads_completed < 2) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    // 输出结果
    if (discriminant >= 0) {
        printf("Root 1: %.2lf\n", root1);
        printf("Root 2: %.2lf\n", root2);
    } else {
        printf("No real roots, discriminant is negative.\n");
    }
    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}