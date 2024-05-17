/*
 * filename: fractals.c
 * date: 2024/05/11 19:44
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#include <stdbool.h>
#include <complex.h>
#include <unistd.h>
#include <stdio.h>

#include "fractals.h"

#include "messages.h"
#include "utils.h"

computation_set_t computation_set;

void set_abort()
{
  computation_set.is_aborted = true;
}

void allow_comp()
{
  computation_set.is_aborted = false;
}

bool is_aborted()
{
  return computation_set.is_aborted;
}

void set_fractal_comp(const message *msg)
{
  computation_set.c = msg->data.set_compute.c_re + msg->data.set_compute.c_im * I;
  computation_set.d_re = msg->data.set_compute.d_re;
  computation_set.d_im = msg->data.set_compute.d_im;
  computation_set.n = msg->data.set_compute.n;
  computation_set.is_aborted = false;
}

void compute_fractal(const message *msg, int pipe_out)
{
  double complex num;
  int computed_len = 0;
  int iters = 0;

  message *computed = my_alloc(sizeof(message));
  uint8_t computed_buf[sizeof(message)];

  computed->type = MSG_COMPUTE_DATA;
  computed->data.compute_data.cid = msg->data.compute.cid;

  for (int r = 0; r < msg->data.compute.n_re; r++) {
    for (int i = 0; i < msg->data.compute.n_im; i++) {
        num = (msg->data.compute.re + computation_set.d_re * r) + ((msg->data.compute.im + computation_set.d_im * i) * I);
        iters = count_convergence(num, computation_set.c, computation_set.n);

        computed->data.compute_data.i_re = r;
        computed->data.compute_data.i_im = i;
        computed->data.compute_data.iter = iters;

        my_assert(fill_message_buf(computed, computed_buf, sizeof(message), &computed_len), __func__, __LINE__, __FILE__);
        write(pipe_out, computed_buf, computed_len);
        computed = my_alloc(sizeof(message));
        computed->type = MSG_COMPUTE_DATA;
        computed->data.compute_data.cid = msg->data.compute.cid;
    }
  }
  free(computed);
  computed = NULL;
}

int count_convergence(double complex num, double complex comp_const, int max_steps)
{

  for (int i = 1; i < max_steps; i++) {
    num *= num;
    num += comp_const;
    if (cabs(num) > 2)
      return i;
  }
  return 0;

}