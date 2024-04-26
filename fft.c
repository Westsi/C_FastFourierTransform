#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <time.h>

#include "mat.h"
#include "vec.h"

#define PI 3.1415926535

void dft(vec x);

int main(int argc, char** argv) {
    srand(time(NULL));
    printf("FFT\n");
    vec v = Vec(3);
    rand_vec(v);
    print_vec(v);
    dft(v);

    free(v);
    return 0;
}

void dft(vec x) {
    /*
    def DFT_slow(x):
        N = x.shape[0]
        n = np.arange(N)
        k = n.reshape((N, 1))
        M = np.exp(-2j * np.pi * k * n / N)
        return np.dot(M, x)
    */
    
    // N = x.shape[0] 
    int shape = x->size;

    // n = np.arange(N)
    vec ranged = py_range(shape);
    print_vec(ranged);

    // k = n.reshape((N, 1))
    // just changes from [0, 1, 2, ... n] to 
    /*
        [0
        1
        2
        .
        .
        .
        n]
    */
   // which is not really necessary here as our vec can be read however we want

    // M = np.exp(-2j * np.pi * k * n / N)
    // for each value in the vector passed into it (-2I * PI * ranged * reshaped_range / shape)
    double complex scalar = -2*I*PI;
    printf("%f%+fi\n", crealf(scalar), cimagf(scalar));
    mat ranged_mat = col_row_vec_mul(ranged, ranged);
    mat_scal_mul(ranged_mat, scalar);
    mat_scal_mul(ranged_mat, 1/(double complex) shape);
    exp_mat(ranged_mat);
    print_mat(ranged_mat);

    // return np.dot(M, x)
    vec r = mat_vec_dot(ranged_mat, x);
    print_vec(r);
}