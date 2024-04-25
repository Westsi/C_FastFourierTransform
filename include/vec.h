#ifndef FFT_VEC_H_
#define FFT_VEC_H_

#include <complex.h>

typedef struct VECN {
    int size;
    double complex elements[];
} vecn;
typedef vecn* vec;

void rand_vec(vec r);
void print_vec(vec vector);
vec Vec(int dimensions);

#endif