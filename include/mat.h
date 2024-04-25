#ifndef FFT_MAT_H_
#define FFT_MAT_H_

#include "vec.h"

typedef struct MATN {
    int rows;
    int cols;
    double complex** elements;
} matn;

typedef matn* mat;

void print_mat(mat matrix);
mat Mat(int rows, int cols);

#endif