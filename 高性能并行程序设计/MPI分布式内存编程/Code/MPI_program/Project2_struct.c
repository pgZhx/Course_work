#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 2048

// 构建一个1D数组，用于表示大小为 m x n 的矩阵
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

// 对以1D数组表示的浮点矩阵进行矩阵乘法
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

typedef struct {
    float* A;
    float* B;
    float* C;
    int m;
    int n;
    int k;
} MatrixData;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int m,n,k;
    m = n = k = SIZE;
    int rows_per_proc = m / size;
    int remaining_rows = m % size;

    MatrixData md;
    md.m = m;
    md.n = n;
    md.k = k;

    md.A = build_Matrix(m, n);
    md.B = build_Matrix(n, k);
    md.C = build_Matrix(m, k);

    float* sub_A = build_Matrix(rows_per_proc, n);
    float* sub_C = build_Matrix(rows_per_proc, k);

    if (rank == 0) {
        fill_Matrix(m, n, md.A, (float)(rand() % 100));
        fill_Matrix(n, k, md.B, (float)(rand() % 100));
    }

    // 创建MPI数据类型
    MPI_Datatype MatrixDataType;
    int blocklengths[3] = {1, 1, 1};
    MPI_Aint offsets[3];
    offsets[0] = offsetof(MatrixData, A);
    offsets[1] = offsetof(MatrixData, B);
    offsets[2] = offsetof(MatrixData, C);
    MPI_Datatype types[3] = {MPI_FLOAT, MPI_FLOAT, MPI_FLOAT};

    MPI_Type_create_struct(3, blocklengths, offsets, types, &MatrixDataType);
    MPI_Type_commit(&MatrixDataType);

    // 广播矩阵B
    MPI_Bcast(md.B, n * k, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // 分发矩阵A
    MPI_Scatter(md.A, rows_per_proc * n, MPI_FLOAT, sub_A, rows_per_proc * n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // 同步所有进程，然后开始计时
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // 执行矩阵乘法
    matrix_multiply(sub_A, md.B, sub_C, rows_per_proc, n, k);

    // 同步所有进程，然后结束计时
    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();
    double local_time_spent = end_time - start_time;

    // 计算所有进程中花费时间最长的一个
    double total_time_spent;
    MPI_Reduce(&local_time_spent, &total_time_spent, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // 收集矩阵C
    MPI_Gather(sub_C, rows_per_proc * k, MPI_FLOAT, md.C, rows_per_proc * k, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("size:%d  ", m);
        printf("Matrix multiplication completed in %f seconds.\n", total_time_spent);
    }

    // 释放分配的内存
    free(sub_A);
    free(sub_C);
    free(md.B);
    if (rank == 0) {
        free(md.A);
        free(md.C);
    }

    MPI_Type_free(&MatrixDataType);
    MPI_Finalize();
    return 0;
}