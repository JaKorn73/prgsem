/*
 * filename: computation.c
 * date: 2024/05/01 08:05 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/
#include <stdlib.h>

#include "computation.h"
#include "utils.h"

static struct {
  double c_re;
  double c_im;
  int n;

  double range_re_min;
  double range_re_max;
  double range_im_min;
  double range_im_max;

  int grid_w;
  int grid_h;

  int cur_x;
  int cur_y;

  double d_re;
  double d_im;

  int nbr_chunks;
  int cid;
  double chunk_re;
  double chunk_im;

  uint8_t chunk_n_re;
  uint8_t chunk_n_im;

  uint8_t *grid;
  bool computing;
  bool done;
  bool abort;
  bool background;
  bool reseted;

} comp = {
  .c_re = -0.4,
  .c_im = 0.6,

  .n = 10,

  .range_re_min = -1.6,
  .range_re_max = 1.6,
  .range_im_min = -1.1,
  .range_im_max = 1.1,

  .grid = NULL,
  .grid_w = 640,
  .grid_h = 480,

  .chunk_n_re = 64,
  .chunk_n_im = 48,

  .computing = false,
  .done = false,
  .abort = false,
  .background = false,
  .reseted = false
};

// Getters for GUI
double get_range_re_min(void)
{
  return comp.range_re_min;
}

double get_range_re_max(void)
{
  return comp.range_re_max;
}

double get_range_im_min(void)
{
  return comp.range_im_min;
}

double get_range_im_max(void)
{
  return comp.range_im_max;
}

double get_c_re(void)
{
  return comp.c_re;
}

double get_c_im(void)
{
  return comp.c_im;
}

int get_n(void)
{
  return comp.n;
}

// Setters for GUI
void set_range_re_min(double new_value)
{
  comp.range_re_min = new_value;
}

void set_range_re_max(double new_value)
{
  comp.range_re_max = new_value;
}

void set_range_im_min(double new_value)
{
  comp.range_im_min = new_value;
}

void set_range_im_max(double new_value)
{
  comp.range_im_max = new_value;
}

void set_c_re(double new_value)
{
  comp.c_re = new_value;
}

void set_c_im(double new_value)
{
  comp.c_im = new_value;
}

void set_n(int new_value)
{
  comp.n = new_value;
}

// Functions for handling user input
void clean_buffer(void)
{
  if (comp.grid)
    free(comp.grid);
  comp.grid = my_calloc(comp.grid_h * comp.grid_w);
}

bool is_computing(void)
{
  return comp.computing;
}

bool is_done(void)
{
  return comp.done;
}

bool is_abort(void)
{
  return comp.abort;
}

bool is_background(void)
{
  return comp.background;
}

void set_grid_size(int w, int h)
{
  comp.grid_w = w; 
  comp.grid_h = h;
  free(comp.grid);
  computation_init();
}

void get_grid_size(int *w, int *h)
{
  *w = comp.grid_w;
  *h = comp.grid_h;
}

void abort_comp(void)
{
  comp.abort = true;
  comp.computing = false;
}

void end_computing(void)
{
  comp.computing = false;
}

void enable_comp(void)
{
  if (comp.abort == true)
    comp.computing = true;
  comp.abort = false;
}

void compute_background(void)
{
  comp.background = true;
}

void reset_cid(void)
{
  if (is_computing()) {
    warn("Cannot reset cid while computing!");
  } else {
    comp.cid = 0;
    comp.reseted = true;
  }
}

void increase_n(void)
{
  comp.n += 10;
}

void decrease_n(void)
{
  if (comp.n <= 0 || (comp.n - 10) <= 0) {
    comp.n = 0;
    warn("n is already 0");
  } else
    comp.n -= 10;
}

// Computation functions
void computation_init(void)
{
  comp.grid = my_alloc(comp.grid_w * comp.grid_h);

  comp.chunk_n_re = comp.grid_w / 10;
  comp.chunk_n_im = comp.grid_h / 10;
  
  comp.d_re = (comp.range_re_max - comp.range_re_min) / (1. * comp.grid_w);
  comp.d_im = -(comp.range_im_max - comp.range_im_min) / (1. * comp.grid_h);

  comp.nbr_chunks = (comp.grid_w * comp.grid_h) / (comp.chunk_n_re * comp.chunk_n_im);
}

void computation_cleanup(void)
{
  if (comp.grid)
    free(comp.grid);

  comp.grid = NULL;
}

bool set_compute(message *msg)
{
  my_assert(msg != NULL, __func__, __LINE__, __FILE__);

  bool ret = !is_computing();
  if (ret) {
    msg->type = MSG_SET_COMPUTE;
    msg->data.set_compute.c_re = comp.c_re;
    msg->data.set_compute.c_im = comp.c_im;
    msg->data.set_compute.d_re = comp.d_re;
    msg->data.set_compute.d_im = comp.d_im;
    msg->data.set_compute.n = comp.n;
    comp.done = false;
  }
  return ret;
}

bool compute(message* msg)
{
  if (!is_computing() || comp.reseted) { // first chunk
    comp.reseted = false;
    comp.cid = 0;
    comp.computing = true;
    comp.cur_x = comp.cur_y = 0; // start computation
    comp.chunk_re = comp.range_re_min; //upper-left
    comp.chunk_im = comp.range_im_max; //upper-left
    msg->type = MSG_COMPUTE;
  } else { //next chunk
    comp.cid += 1;
    if (comp.cid < comp.nbr_chunks) {
      comp.cur_x += comp.chunk_n_re;
      comp.chunk_re += comp.chunk_n_re * comp.d_re;
      if (comp.cur_x >= comp.grid_w) {
        comp.cur_x = 0;
        comp.chunk_re = comp.range_re_min;
        comp.chunk_im += comp.chunk_n_im * comp.d_im;
        comp.cur_y += comp.chunk_n_im;
      }
      msg->type = MSG_COMPUTE;
    } else { //all has been compute
      comp.computing = false;
      comp.background = false;
    }
  }

  if (comp.computing && msg->type == MSG_COMPUTE) {
    msg->data.compute.cid = comp.cid;
    msg->data.compute.re = comp.chunk_re;
    msg->data.compute.im = comp.chunk_im;
    msg->data.compute.n_re = comp.chunk_n_re;
    msg->data.compute.n_im = comp.chunk_n_im;
  }

  return is_computing();
}

void update_image(int w, int h, unsigned char *img)
{
  my_assert(img && comp.grid && w == comp.grid_w && h == comp.grid_h, __func__, __LINE__, __FILE__);
  for (int i = 0; i < w * h; ++i) {
    const double t = 1. * comp.grid[i] / (comp.n + 1.0);
    *(img++) = 9 * (1 - t) * t * t * t * 255;
    *(img++) = 15 * (1 - t) * (1 - t) * t * t * 255;
    *(img++) = 8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255;
  }
}

void update_data(const msg_compute_data *compute_data)
{
  my_assert(comp.grid && compute_data != NULL, __func__, __LINE__, __FILE__);
  if (compute_data->cid == comp.cid) {
    const int idx = comp.cur_x + compute_data->i_re + (comp.cur_y + compute_data->i_im) * comp.grid_w;

    if (idx >= 0 && idx < (comp.grid_w * comp.grid_h)) {
      comp.grid[idx] = compute_data->iter;
    }
    if ((comp.cid + 1) >= comp.nbr_chunks && (compute_data->i_re + 1) == comp.chunk_n_re && (compute_data->i_im + 1) == comp.chunk_n_im)
    {
      comp.done = true;
      comp.computing = false;
    }
  } else {
    warn("received chunk with unexpected chunk id (cid)");
  }
}
