#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <time.h>

#include "mat.h"
#include "vec.h"

#define PI 3.1415926535

int main(int argc, char** argv) {
    srand(time(NULL));
    printf("FFT\n");
    vec v = Vec(1024);
    rand_vec(v);
    print_vec(v);
    mat m = Mat(2, 3);
    // m->elements[row][col] = val;
    m->elements[1][1] = 5678;
    m->elements[0][1] = 6969;
    print_mat(m);
    free(v);
    free(m);
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
   // which is not really necessary here as it can be read however we want
}