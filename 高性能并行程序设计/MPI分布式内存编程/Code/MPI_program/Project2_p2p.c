#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 2048


// 构建一个大小为 m x n 的矩阵，使用一维数组表示
float* build_Matrix(int m, int n) {
    return (float*)malloc(m * n * sizeof(float));
}

// 使用固定值或随机值填充矩阵
void fill_Matrix(int m, int n, float* A, float value) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i * n + j] = value; // 分配固定值或生成随机值
        }
    }
}

// 对以一维数组表示的浮点矩阵进行矩阵乘法
void matrix_multiply(float* A, float* B, float* C, int m, int n, int k) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            C[i * k + j] = 0.0f;
            for (int p = 0; p < n; p++) {
                C[i * k + j] += A[i * n + p] * B[p * k + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv); // 初始化MPI环境
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // 获取当前进程的排名
    MPI_Comm_size(MPI_COMM_WORLD, &size); // 获取进程总数
    int m,n,k;
    m = n = k = SIZE; // 定义矩阵的行数和列数
    int rows_per_proc = m / size; // 每个进程处理的行数
    int remaining_rows = m % size; // 剩余的行数，用于处理不能均匀分配的情况

    float* A = NULL; // 矩阵A
    float* B = build_Matrix(n, k); // 矩阵B
    float* C = NULL; // 结果矩阵C
    float* sub_A = build_Matrix(rows_per_proc, n); // 子矩阵A，存储当前进程的矩阵A部分
    float* sub_C = build_Matrix(rows_per_proc, k); // 子矩阵C，存储当前进程的矩阵C部分

    if (rank == 0) {
        A = build_Matrix(m, n); // 根进程构建完整的矩阵A
        C = build_Matrix(m, k); // 根进程构建完整的矩阵C

        // 使用随机浮点值填充矩阵A和B
        fill_Matrix(m, n, A, (float)(rand() % 100));
        fill_Matrix(n, k, B, (float)(rand() % 100));

        // 根进程将矩阵B发送给其他所有进程
        for (int i = 1; i < size; i++) {
            MPI_Send(B, n * k, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // 非根进程接收矩阵B
        MPI_Recv(B, n * k, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // 使用点对点通信分发矩阵A
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(A + i * rows_per_proc * n, rows_per_proc * n, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }
        // 根进程保留矩阵A的自己的部分
        for (int i = 0; i < rows_per_proc * n; i++) {
            sub_A[i] = A[i];
        }
    } else {
        // 非根进程接收矩阵A的一部分
        MPI_Recv(sub_A, rows_per_proc * n, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // 同步所有进程，然后开始计时
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // 执行矩阵乘法
    matrix_multiply(sub_A, B, sub_C, rows_per_proc, n, k);

    // 同步所有进程，然后结束计时
    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();
    double local_time_spent = end_time - start_time;

    // 计算所有进程中花费时间最长的一个
    double total_time_spent;
    MPI_Reduce(&local_time_spent, &total_time_spent, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // 使用点对点通信收集结果到矩阵C
    if (rank == 0) {
        for (int i = 0; i < rows_per_proc * k; i++) {
            C[i] = sub_C[i];
        }
        for (int i = 1; i < size; i++) {
            MPI_Recv(C + i * rows_per_proc * k, rows_per_proc * k, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else {
        MPI_Send(sub_C, rows_per_proc * k, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        printf("size:%d  ", m);
        printf("Matrix multiplication completed in %f seconds.\n", total_time_spent);
    }

    // 释放分配的内存
    free(sub_A);
    free(sub_C);
    free(B);
    if (rank == 0) {
        free(A);
        free(C);
    }

    MPI_Finalize(); // 终止MPI环境
    return 0;
}