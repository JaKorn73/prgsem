/*
 * filename: fractals.c
 * date: 2024/05/11 19:44 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#include <stdbool.h>
#include <complex.h>

#include "messages.h"

typedef struct {
   double complex c; // complex constant
   double d_re;  // increment in the x-coords
   double d_im;  // increment in the y-coords
   uint8_t n;    // number of iterations per each pixel
   bool is_aborted;
} computation_set_t;

void set_fractal_comp(const message *msg);
void compute_fractal(const message *msg, int pipe_out);
int count_convergence(double complex num, double complex comp_const, int max_steps);
bool is_aborted();
void set_abort();
void allow_comp();