#ifndef FFT_MAT_H_
#define FFT_MAT_H_

#include "vec.h"

typedef struct MATN {
    int size;
    vec elements[];
} mat;

#endif