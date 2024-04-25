#include <stdlib.h>
#include <stdio.h>

#include "mat.h"

void print_mat(mat matrix) {
    printf("Mat {\n\trows: %d x cols: %d\n\telements:\n", matrix->rows, matrix->cols);
    for (int i=0;i<matrix->cols;i++) {
        printf("\t");
        for (int j=0;j<matrix->rows;j++) {
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