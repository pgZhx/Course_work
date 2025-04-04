//
// Created by 26618 on 2024/10/9.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef LAB2_MATRIX_MULTIPLY_H
#define LAB2_MATRIX_MULTIPLY_H
float* build_Matrix(int m, int n);
void fill_Matrix(int m, int n, float* A, float value);
void matrix_multiply(float* A, float* B, float* C, int m, int n, int k);
#endif //LAB2_MATRIX_MULTIPLY_H
