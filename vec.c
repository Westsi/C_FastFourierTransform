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

vec slice(vec v, int start, int stop, int step) {
    // A C implementation of the python/numpy array slicing format
    // v[start:stop:step]
    // If slice indices passed as -1, they default to start=0, stop=v->size, step=1
    if (step == -1) step = 1;
    if (stop == -1) stop = v->size;
    if (start == -1) start = 0;

    // create new vector with enough space for the elements that will be sliced out
    int vec_len = (stop-start) % 2 == 0 ? (stop-start)/step : (stop+1-start)/step;
    vec r = Vec(vec_len);
    int j = 0;
    for (int i=start;i<stop;i+=step) {
        r->elements[j] = v->elements[i];
        j++;
    }
    return r;
}

void vec_scal_mul(vec v, double complex s) {
    for (int i=0;i<v->size;i++) {
        v->elements[i] = v->elements[i] * s;
    }
}

vec vec_elwise_mul(vec f, vec s) {
    if (f->size != s->size) {
        printf("Vectors need to have same sizes for element wise multiplication");
        exit(1);
    }
    vec r = Vec(f->size);
    for (int i=0;i<f->size;i++) {
        r->elements[i] = f->elements[i] * s->elements[i];
    }
    return r;
}

vec vec_add(vec f, vec s) {
    if (f->size != s->size) {
        printf("Vectors need to have same sizes for addition");
        exit(1);
    }
    vec r = Vec(f->size);
    for (int i=0;i<f->size;i++) {
        r->elements[i] = f->elements[i] + s->elements[i];
    }
    return r;
}

vec vec_concat(vec f, vec s) {
    vec r = Vec(f->size + s->size);
    for (int i=0;i<f->size;i++) {
        r->elements[i] = f->elements[i];
    }
    for (int j=0;j<s->size;j++) {
        r->elements[f->size+j] = s->elements[j];
    }
    return r;
}

void vec_exp(vec v) {
    for (int i=0;i<v->size;i++) {
        v->elements[i] = cexp(v->elements[i]);
    }
}