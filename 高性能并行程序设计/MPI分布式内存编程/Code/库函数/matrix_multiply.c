//
// Created by 26618 on 2024/10/9.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix_multiply.h"
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