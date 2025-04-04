#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <omp.h>

// Build a matrix of size m x n
float** build_Matrix(int m, int n) {
    float** A = (float**)malloc(m * sizeof(float*));
    for (int i = 0; i < m; i++) {
        A[i] = (float*)malloc(n * sizeof(float));
    }
    return A;
}

// Fill the matrix with a fixed value or a random value
void fill_Matrix(int m, int n, float** A, float value) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = value; // Assign a fixed value or generate a random one
        }
    }
}

// Block matrix multiplication for float matrices
void block_matrix_multiply(float** A, float** B, float** C, int m, int n, int k, int block_size) {
    // 初始化结果矩阵 C 为零
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = 0.0f;
        }
    }

    // 分块矩阵乘法
    #pragma omp parallel for collapse(3)
    for (int ii = 0; ii < m; ii += block_size) {
        for (int jj = 0; jj < k; jj += block_size) {
            for (int pp = 0; pp < n; pp += block_size) {
                // 对每个分块进行计算
                for (int i = ii; i < (ii + block_size < m ? ii + block_size : m); i++) {
                    for (int p = pp; p < (pp + block_size < n ? pp + block_size : n); p++) {
                        for (int j = jj; j < (jj + block_size < k ? jj + block_size : k); j++) {
                            C[i][j] += A[i][p] * B[p][j];
                        }
                    }
                }
            }
        }
    }
}


int main() {
    int m = 2048, n = 2048, k = 2048;
    int block_size = 128; // 分块大小可以调整，譬如 32, 64, 128 等

    float** A = build_Matrix(m, n);
    float** B = build_Matrix(n, k);
    float** C = build_Matrix(m, k);

    // Fill matrices with random float values for testing
    printf("Filling Matrix A (2048x2048) with random values\n");
    fill_Matrix(m, n, A, (float)(rand() % 100)); // Fill A with random float values
    printf("Filling Matrix B (2048x2048) with random values\n");
    fill_Matrix(n, k, B, (float)(rand() % 100)); // Fill B with random float values

    clock_t start = clock();
    block_matrix_multiply(A, B, C, m, n, k, block_size);
    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // Uncomment this line if you want to print the matrix (can be very large)
    // print_Matrix(C, m, k);

    printf("Block Matrix multiplication completed in %f seconds.\n", time_spent);
    printf("Press Enter to continue...");
    _getch();  // 等待用户按下一个键
    printf("\nContinuing...");

    // Free allocated memory
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
