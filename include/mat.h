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
mat col_row_vec_mul(vec col_vec, vec row_vec);
void mat_scal_mul(mat m, double complex s);
void exp_mat(mat m);
vec mat_vec_dot(mat m, vec v);

#endif