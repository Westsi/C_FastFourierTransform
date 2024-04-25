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
    vec v = Vec(2);
    rand_vec(v);
    print_vec(v);
    printf("Hi!\n");
    return 0;
}