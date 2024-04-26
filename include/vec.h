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
vec py_range(int shape);
vec slice(vec v, int start, int stop, int step);
void vec_scal_mul(vec v, double complex s);
vec vec_elwise_mul(vec f, vec s);
vec vec_add(vec f, vec s);
vec vec_concat(vec f, vec s);
void vec_exp(vec v);

#endif