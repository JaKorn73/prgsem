/*
 * filename: gui.c
 * date: 2024/05/01 08:50 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#include <SDL.h>
#include <math.h>

#include "gui.h"

#include "xwin_sdl.h"
#include "utils.h"
#include "computation.h"
#include "event_queue.h"
#include "menu.h"
#include "menu_parameters.h"


int menu_id = 0;

static struct {
  int w;
  int h;
  unsigned char *img;
} gui = { .img = NULL };


void gui_init(void)
{
  get_grid_size(&gui.w, &gui.h);
  gui.img = my_alloc(gui.w * (gui.h + MENU_HEIGHT)* 3);
  generate_menu_button();
  my_assert(xwin_init(gui.w, gui.h) == 0, __func__, __LINE__, __FILE__);
  xwin_redraw(gui.w, gui.h, gui.img);
}

void gui_cleanup(void)
{
  if (gui.img)
    free(gui.img);

  gui.img = NULL;
  xwin_close();
}

void gui_refresh(void)
{
  if (gui.img) {
    update_image(gui.w, gui.h, gui.img);
    fprintf(stderr, "GUI REFRESH: %d %d\n\n\n", gui.w, gui.h);
    xwin_redraw(gui.w, gui.h, gui.img);
  }
}

void gui_resize(int w, int h)
{
  gui.w = w;
  gui.h = h;
  gui_cleanup();
  gui_init();

}

int round_pixels(int pixel)
{
  int len = log10(pixel);
  float div = pow(10, len);
  return ceil(pixel / div) * div;
}

void generate_menu_button(void)
{
  for (int i = (gui.w * gui.h * 3); i < ((gui.w * MENU_HEIGHT) + (gui.w * gui.h)) * 3; i += 3) {
    gui.img[i] = 255;
    gui.img[i + 1] = 0;
    gui.img[i + 2] = 0;
  }
}

void* gui_win_thread(void *d)
{
  bool quit = false;
  SDL_Event sdl_ev;
  event ev;
  int resize_w = 640;
  int resize_h = 480;

  while(!quit) {
  ev.type = EV_TYPE_NUM;

  if (SDL_PollEvent(&sdl_ev)) {
    if (sdl_ev.window.windowID == menu_id) {
      handle_menu_event(sdl_ev, &menu_id);
    } else {
      if (sdl_ev.type == SDL_KEYDOWN) {
        switch(sdl_ev.key.keysym.sym) {
          case SDLK_a:
            ev.type = EV_ABORT;
            break;

          case SDLK_c:
            if (!is_computing()) {
              ev.type = EV_COMPUTE;
            } else {
              warn("Already computing");
            }
            break;

          case SDLK_g:
            ev.type = EV_GET_VERSION;
            break;

          case SDLK_l:
            ev.type = EV_CLEAR_BUFFER;
            break;

          case SDLK_p:
            ev.type = EV_SHOW;
            break;

          case SDLK_q:
            set_quit();
            ev.type = EV_QUIT;
            break;

          case SDLK_r:
            ev.type = EV_REFRESH;
            break;

          case SDLK_s:
            ev.type = EV_SET_COMPUTE;
            break;

          case SDLK_0:
            fprintf(stderr, "Resize w: %d, Resize h: %d\n", resize_w, resize_h);
            set_grid_size(resize_w, resize_h);
            gui_resize(resize_w, resize_h);
            if (is_done()) {
              ev.type = EV_REFRESH;
            }
            break;

          case SDLK_1:
            info("Computation in the background started");
            ev.type = EV_COMPUTE_CPU;
            break;

          case ARROW_UP:
            increase_n();
            break;

          case ARROW_DOWN:
            decrease_n();
            break;
        }
      } else if (sdl_ev.type == SDL_KEYUP) {
        info("keyup");
      } else if (sdl_ev.type == SDL_WINDOWEVENT) {
        if (sdl_ev.window.event == SDL_WINDOWEVENT_CLOSE) {
          set_quit();
          ev.type = EV_QUIT;
          queue_push(ev);
          break;
        } else if (sdl_ev.window.event == SDL_WINDOWEVENT_RESIZED) {
          // fprintf(stderr, "Resize to w: %d, h: %d\n", sdl_ev.window.data1, sdl_ev.window.data2);
          if (is_computing()) {
            ev.type = EV_ABORT;
            queue_push(ev);
          }

          resize_w = round_pixels(sdl_ev.window.data1);
          resize_h = round_pixels(sdl_ev.window.data2);
        }
      } else if (sdl_ev.type == SDL_MOUSEBUTTONDOWN) {
        fprintf(stderr, "X coord: %d\nY coord: %d\n", sdl_ev.button.x, sdl_ev.button.y);
        if (sdl_ev.button.y > gui.h) {
          if (menu_id == 0) {
            menu_id = menu_init();
          } else {
            warn("Menu is already opened!");
          }
        }
      }
    }
    if (ev.type != EV_TYPE_NUM) {
      queue_push(ev);
    }
  }
  SDL_Delay(SDL_EVENT_POLL_WAIT_MS);

  quit = is_quit();
  }
  return NULL;
}
