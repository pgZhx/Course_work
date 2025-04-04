import java.util.Random;
import java.util.Scanner;

public class Project1_j {
    // Build a matrix of size m x n with float elements
    public static float[][] buildMatrix(int m, int n) {
        float[][] matrix = new float[m][n];
        return matrix;
    }

    // Fill the matrix with random float values between 0.0 and 99.99
    public static void fillMatrix(int m, int n, float[][] matrix) {
        Random rand = new Random();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                matrix[i][j] = rand.nextFloat() * 100; // Random float values between 0.0 and 99.99
            }
        }
    }

    // Matrix multiplication for float matrices
    public static float[][] matrixMultiply(float[][] A, float[][] B, int m, int n, int k) {
        float[][] C = buildMatrix(m, k);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < k; j++) {
                C[i][j] = 0.0f;
                for (int p = 0; p < n; p++) {
                    C[i][j] += A[i][p] * B[p][j];
                }
            }
        }
        return C;
    }

    // Print the matrix with float values
    public static void printMatrix(float[][] matrix, int m, int n) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                System.out.print(matrix[i][j] + " ");
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        int m = 2048, n = 2048, k = 2048;

        // Build matrices A, B, and C
        float[][] A = buildMatrix(m, n);
        float[][] B = buildMatrix(n, k);
        float[][] C = buildMatrix(m, k);

        // Fill matrices A and B with random float values
        fillMatrix(m, n, A);
        System.out.println("Filling Matrix A (2048x2048) with random values");
        fillMatrix(n, k, B);
        System.out.println("Filling Matrix B (2048x2048) with random values");

        // Measure time for matrix multiplication
        long startTime = System.currentTimeMillis();
        C = matrixMultiply(A, B, m, n, k);
        long endTime = System.currentTimeMillis();

        double timeSpent = (endTime - startTime) / 1000.0;
        System.out.println("Matrix multiplication completed in " + timeSpent + " seconds.");

        // Wait for user input to continue
        Scanner scanner = new Scanner(System.in);
        System.out.println("Press Enter to continue...");
        scanner.nextLine();  // Wait for user to press Enter
    }
}
