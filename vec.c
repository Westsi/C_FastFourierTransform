#include <stdlib.h>
#include <stdio.h>

#include "vec.h"

void rand_vec(vec r) {
    for (int i=0;i<r->size;i++) {
        // double complex val = rand();
        r->elements[i] = 69;
    }
}

void print_vec(vec vector) {
    printf("Vec {\t\nsize: %d\t\nelements:\n", vector->size);
    for (int i=0;i<vector->size;i++) {
        printf("\t%f\n", crealf(vector->elements[i]));
        // printf("\t%f%+fi\n", crealf(vector->elements[i]), cimagf(vector->elements[i]));
    }
    printf("}\n");
}

vec Vec(int dimensions) {
    vec r =  malloc(sizeof(vec) + dimensions * sizeof(double complex));
    r->size = dimensions;
    return r;
}