#include <stdlib.h>
#include <stdio.h>

#include "vec.h"

void rand_vec(vec r) {
    for (int i=0;i<r->size;i++) {
        double complex val = rand();
        r->elements[i] = val;
    }
}

void print_vec(vec vector) {
    printf("Vec {\n\tsize: %d\n\telements:\n", vector->size);
    for (int i=0;i<vector->size;i++) {
        printf("\t%f%+fi\n", crealf(vector->elements[i]), cimagf(vector->elements[i]));
    }
    printf("}\n");
}

vec Vec(int dimensions) {
    vec r =  malloc(sizeof(vec) + dimensions * sizeof(double complex));
    r->size = dimensions;
    return r;
}

vec py_range(int shape) {
    vec r = Vec(shape);
    for (int i=0;i<shape;i++) {
        r->elements[i] = i;
    }
    return r;
}