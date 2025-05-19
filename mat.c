#include <stdlib.h>
#include <stdio.h>
#include <complex.h>

#include "mat.h"

void print_mat(mat matrix) {
    printf("Mat {\n\trows: %d x cols: %d\n\telements:\n", matrix->rows, matrix->cols);
    for (int i=0;i<matrix->rows;i++) {
        printf("\t");
        for (int j=0;j<matrix->cols;j++) {
            double complex el = matrix->elements[i][j];
            printf("%f%+fi\t", crealf(el), cimagf(el));
        }
        printf("\n");
    } 
    printf("}\n");
}


mat Mat(int rows, int cols) {
    mat r = malloc(sizeof(matn));
    r->rows = rows;
    r->cols = cols;
    r->elements = malloc(cols * sizeof(double complex*));
    for (int i = 0; i < cols; i++) {
        r->elements[i] = malloc(rows * sizeof(double complex));
    }
    return r;
}

mat col_row_vec_mul(vec col_vec, vec row_vec) {
    // create new matrix
    // size of column vector is number of rows
    // size of row vector is number of columns
    mat m = Mat(col_vec->size, row_vec->size);
    for (int i=0;i<col_vec->size;i++) {
        for (int j=0;j<row_vec->size;j++) {
            m->elements[i][j] = col_vec->elements[i] * row_vec->elements[j];
        }
    }
    return m;
}

void mat_scal_mul(mat m, double complex s) {
    for (int i=0;i<m->cols;i++) {
        for (int j=0;j<m->rows;j++) {
            m->elements[i][j] = m->elements[i][j] * s;
        }
    }
}

void exp_mat(mat m) {
    for (int i=0;i<m->cols;i++) {
        for (int j=0;j<m->rows;j++) {
            m->elements[i][j] = cexp(m->elements[i][j]);
        }
    }
}

vec mat_vec_dot(mat m, vec v) {
    /*
    m = [a, b]
        [c, d]
    v = [x, y]
    m dot v = [ax+by, cx+dy]
    */
    int rows = m->rows;
    vec r = Vec(rows);
    for (int i=0;i<m->rows;i++) {
        for (int j=0;j<m->cols;j++) {
            double complex val = v->elements[j] * m->elements[i][j];
            // printf("\t%f%+fi\n", crealf(val), cimagf(val));
            r->elements[i] += val;
        }
    }   
    return r;
}