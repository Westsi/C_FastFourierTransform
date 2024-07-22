#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "mat.h"
#include "vec.h"
#include "mic.h"

#define PI 3.1415926535

vec fft(vec x);
vec dft(vec x);
void find_signals(vec fourier, double sampling_rate);
vec fft_freq(int window, double spacing);

int main(int argc, char** argv) {
    // AudioObjectID* micdata = malloc(sizeof(AudioObjectID)*128);
    // defaultMicrophone(micdata);
    record();

    srand(time(NULL));

    vec v = load_vec("signal.txt");

    printf("Input vector:\n");
    print_vec(v);

    vec r = fft(v);

    printf("FFT of input:\n");
    print_vec(r);
    printf("Size of output: %d\n", r->size);

    find_signals(r, 128.0);

    free(v);
    free(r);
    return 0;
}

void find_signals(vec fourier, double sampling_rate) {
    // calculate threshold
    double threshold = 0;
    for (int i=0;i<fourier->size;i++) {
        if (cabs(fourier->elements[i]) > threshold) {
            threshold = cabs(fourier->elements[i]);
        }
    }
    threshold /= 10000;

    vec frequencies = fft_freq(fourier->size, 1/sampling_rate);

    // zero all values less than threshold in v
    for (int i=0;i<fourier->size;i++) {
        if (cabs(fourier->elements[i]) < threshold) {
            continue;
        }
        double complex el = fourier->elements[i];

        double amp = 2*cabs(el)/fourier->size;

        // convert received angle to degrees and add 90 to get the correct value
        double phase_angle = (atan(cimag(el)/creal(el))*180/PI) + 90;

        // I don't know if this can be relied upon to be true!
        // seems to work in all tests i've done - but IDK if it is proved that it always is
        double freq = creal(frequencies->elements[i]);
        // check if frequency is negative - if so, do not print as positive one will be printed
        if (freq < 0) {
            continue;
        }
        printf("Found signal with amplitude %f, phase angle %f°, and frequency %f\n", amp, phase_angle, freq);
    }


}

vec fft_freq(int window, double spacing) {
    // spacing is 1/sampling rate
    if (window % 2 == 0) {
        vec first_half = py_range(((int) window/2));
        vec second_half = Vec((int) window/2);
        int i = 0;
        for (int j=(int)-window/2;j<=-1;j++,i++) {
            second_half->elements[i] = j;
        }
        vec r = vec_concat(first_half, second_half);
        free(first_half);
        free(second_half);
        vec_scal_mul(r, 1/(window*spacing)); // divided by spacing*window
        return r;
    }
    vec first_half = py_range((int) (window-1)/2);
    vec second_half = Vec((int) window/2);
    int i = 0;
    for (int j=(int)-(window-1)/2;j<=-1;j++,i++) {
        second_half->elements[i] = j;
    }
    vec r = vec_concat(first_half, second_half);
    free(first_half);
    free(second_half);
    vec_scal_mul(r, 1/(window*spacing)); // divided by spacing*window
    return r;
}

vec fft(vec x) {
    /*
    def FFT(x):
        x = np.asarray(x, dtype=float)
        N = x.shape[0]
        
        if N % 2 > 0:
            raise ValueError("size of x must be a power of 2")
        elif N <= 32:  # this cutoff should be optimized
            return DFT_slow(x)
        else:
            X_even = FFT(x[::2])
            X_odd = FFT(x[1::2])
            factor = np.exp(-2j * np.pi * np.arange(N) / N)
            return np.concatenate([X_even + factor[:N / 2] * X_odd,
                                X_even + factor[N / 2:] * X_odd])
    */
    // N = x.shape[0]
    int shape = x->size;
    
    if (shape % 2 > 0) {
        printf("Size of vector must be a power of 2.");
        exit(1);
    }
    if (shape <= 16) { //TODO: optimise this cutoff value
        return dft(x);
    } else {
        // X_even = FFT(x[::2])
    // X_odd = FFT(x[1::2])
    vec even_x_indices = slice(x, 0, -1, 2);
    vec odd_x_indices = slice(x, 1, -1, 2);
    vec even_x = fft(even_x_indices);
    vec odd_x = fft(odd_x_indices);
    free(even_x_indices);
    free(odd_x_indices);

    // factor = np.exp(-2j * np.pi * np.arange(N) / N)
    double complex scalar = -2*I*PI/shape;
    vec ranged = py_range(shape);
    vec_scal_mul(ranged, scalar);
    vec_exp(ranged);

    // return np.concatenate([X_even + factor[:N / 2] * X_odd, X_even + factor[N / 2:] * X_odd])
    // even_x + slice(ranged, 0, shape/2, 1) * odd_x
        // vector vector addition, vector vector elementwise multiplication
        // r[0] = vo[0] + vt[0], r[0] = vo[0] * vt[0] 
    // even_x + slice(ranged, shape/2, -1, 1) * odd_x
    // concatenate
        // concatenate([2,4], [1,3]) --> [2, 4, 1, 3]
    vec ranged_first_half = slice(ranged, 0, shape/2, 1);
    vec first_half_multiplied = vec_elwise_mul(ranged_first_half, odd_x);
    vec first_half_added = vec_add(even_x, first_half_multiplied);
    free(ranged_first_half);
    free(first_half_multiplied);

    vec ranged_second_half = slice(ranged, shape/2, -1, 1);
    vec second_half_multiplied = vec_elwise_mul(ranged_second_half, odd_x);
    vec second_half_added = vec_add(even_x, second_half_multiplied);
    free(ranged_second_half);
    free(second_half_multiplied);

    vec r = vec_concat(first_half_added, second_half_added);
    free(first_half_added);
    free(second_half_added);
    free(ranged);
    free(even_x);
    free(odd_x);
    return r;
    }
}

vec dft(vec x) {
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
    // print_vec(ranged);

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
    double complex scalar = -2*I*PI/shape;
    // printf("%f%+fi\n", crealf(scalar), cimagf(scalar));
    mat ranged_mat = col_row_vec_mul(ranged, ranged);
    mat_scal_mul(ranged_mat, scalar);
    exp_mat(ranged_mat);
    // print_mat(ranged_mat);

    // return np.dot(M, x)
    vec r = mat_vec_dot(ranged_mat, x);
    free(ranged);
    free(ranged_mat);
    return r;
}
