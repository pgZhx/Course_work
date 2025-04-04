#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

float** build_matrix(int m, int n) {
    float** matrix = (float**)malloc(m * sizeof(float*));
    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        matrix[i] = (float*)malloc(n * sizeof(float));
    }
    return matrix;
}
void fill_matrix(int m, int n, float** A) {
    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = (float)(rand() % 10); // 生成0到10之间的随机浮点数
        }
    }
}
void multiply_matrix(float** A, float** B, float** C, int m, int n, int k) {
    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = 0.0;
            for (int p = 0; p < n; p++) {
                C[i][j] += A[i][p] * B[p][j];
            }
        }
    }
}

int main() {
    int size;
    int thread;
    scanf("%d %d", &size, &thread);

    omp_set_num_threads(thread); // 设置线程数

    int m = size, n = size, k = size;
    float** A = build_matrix(m, n);
    float** B = build_matrix(n, k);
    float** C = build_matrix(m, k);

    fill_matrix(m, n, A);
    fill_matrix(n, k, B);

    double start = omp_get_wtime();
    multiply_matrix(A, B, C, m, n, k);
    double end = omp_get_wtime();

    printf("Size:%d, threads:%d, Matrix_multiply takes %.3lf s\n", size, thread, end - start);

    // 释放内存
    for (int i = 0; i < m; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}