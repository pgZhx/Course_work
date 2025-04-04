#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000

// 全局变量
int a[ARRAY_SIZE];
int global_sum = 0;
int global_index = 0;
int num_threads = 8;
// 互斥锁
pthread_mutex_t lock;

// 线程函数
void* sum_array(void* arg) {
    int sum = 0;
    int i;
    for (i = 0; i < ARRAY_SIZE / num_threads; i++) {
        // 获取互斥锁
        pthread_mutex_lock(&lock);
        // 检查是否还有元素需要处理
        if (global_index < ARRAY_SIZE) {
            int index = global_index++;
            // 释放互斥锁
            pthread_mutex_unlock(&lock);

            sum += a[index];
        } else {
            // 释放互斥锁
            pthread_mutex_unlock(&lock);
            break;
        }
    }
    // 对共享变量加锁，以更新总和
    pthread_mutex_lock(&lock);
    global_sum += sum;
    pthread_mutex_unlock(&lock);
    return NULL;
}//动态分配的方式

int main() {
    pthread_t threads[num_threads];
    int result;
    // 初始化互斥锁
    pthread_mutex_init(&lock, NULL);
    // 初始化数组
    for (int i = 0; i < ARRAY_SIZE; i++) {
        a[i] = i;
    }
    // 创建线程
    for (int i = 0; i < num_threads; i++) {
        result = pthread_create(&threads[i], NULL, sum_array, NULL);
        if (result) {
            printf("Error creating thread\n");
            exit(-1);
        }
    }
    // 等待线程结束
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    // 输出结果
    printf("Sum: %d\n", global_sum);
    // 销毁互斥锁
    pthread_mutex_destroy(&lock);
    return 0;
}