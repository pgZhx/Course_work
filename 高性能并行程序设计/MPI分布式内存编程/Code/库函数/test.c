//
// Created by 26618 on 2024/10/9.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix_multiply.h"
#include <mpi.h>
#define SIZE 1024
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
    }

    // 广播矩阵B
    MPI_Bcast(B, n * k, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // 分发矩阵A
    MPI_Scatter(A, rows_per_proc * n, MPI_FLOAT, sub_A, rows_per_proc * n, MPI_FLOAT, 0, MPI_COMM_WORLD);

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

    // 收集矩阵C
    MPI_Gather(sub_C, rows_per_proc * k, MPI_FLOAT, C, rows_per_proc * k, MPI_FLOAT, 0, MPI_COMM_WORLD);

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
