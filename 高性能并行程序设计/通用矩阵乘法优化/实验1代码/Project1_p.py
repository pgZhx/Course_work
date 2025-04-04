import time
import random

def build_matrix(m, n):
    """创建一个 m x n 的矩阵，并初始化为零。"""
    return [[0.0 for _ in range(n)] for _ in range(m)]  # 使用 0.0 来初始化浮点数

def fill_matrix(m, n, matrix):
    """用随机数填充矩阵。"""
    for i in range(m):
        for j in range(n):
            matrix[i][j] = random.uniform(0, 99)  # 使用 random.uniform 生成浮点数

def matrix_multiply(A, B):
    """执行矩阵乘法 A * B。"""
    m, n, k = len(A), len(A[0]), len(B[0])
    C = build_matrix(m, k)  # 确保 C 矩阵也使用浮点数初始化
    for i in range(m):
        for j in range(k):
            C[i][j] = 0.0  # 初始化为浮点数 0.0
            for p in range(n):
                C[i][j] += A[i][p] * B[p][j]
    return C

def print_matrix(matrix):
    """打印矩阵。"""
    for row in matrix:
        print(' '.join(map(lambda x: f"{x:.2f}", row)))  # 格式化输出为两位小数的浮点数

def main():
    m, n, k = 2048, 2048, 2048

    A = build_matrix(m, n)
    B = build_matrix(n, k)
    C = build_matrix(m, k)

    # Fill matrices with random values for testing
    print("Filling Matrix A (2048x2048) with random values")
    fill_matrix(m, n, A)

    print("Filling Matrix B (2048x2048) with random values")
    fill_matrix(n, k, B)

    start_time = time.time()
    C = matrix_multiply(A, B)
    end_time = time.time()

    time_spent = end_time - start_time

    print(f"Matrix multiplication completed in {time_spent:.6f} seconds.")
    input("Press Enter to continue...")

if __name__ == "__main__":
    main()