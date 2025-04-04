#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
std::vector<std::vector<int>> build_Matrix(int m, int n) {
    return std::vector<std::vector<int>>(m, std::vector<int>(n, 0));
}

void fill_Matrix(int m, int n, std::vector<std::vector<int>>& A) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = std::rand() % 100; // Assign a random value between 0 and 9
        }
    }
}

std::vector<std::vector<int>> matrix_multiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, int m, int n, int k) {
    std::vector<std::vector<int>> C(m, std::vector<int>(k, 0));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            for (int p = 0; p < n; p++) {
                C[i][j] += A[i][p] * B[p][j];
            }
        }
    }
    return C;
}

void print_Matrix(const std::vector<std::vector<int>>& matrix, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int m = 2048, n = 2048, k = 2048;

    std::vector<std::vector<int>> A = build_Matrix(m, n);
    std::vector<std::vector<int>> B = build_Matrix(n, k);
    std::vector<std::vector<int>> C;

    std::cout << "Filling Matrix A (2048x2048) with random values" << std::endl;
    fill_Matrix(m, n, A);

    std::cout << "Filling Matrix B (2048x2048) with random values" << std::endl;
    fill_Matrix(n, k, B);

    clock_t start = clock();
    C = matrix_multiply(A, B, m, n, k);
    clock_t end = clock();

    double time_spent = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    std::cout << "Matrix multiplication completed in " << time_spent << " seconds." << std::endl;
    std::cout << "按任意键继续..." << std::endl;
    _getch();  // 等待用户输入一个字符
    // Uncomment to print the resulting matrix C
    // std::cout << "Resultant Matrix C (1000x500):" << std::endl;
    // print_Matrix(C, m, k);

    return 0;
}