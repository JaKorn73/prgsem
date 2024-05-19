/*
 * filename: menu.c
 * date: 2024/05/14 11:04 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "gui.h"
#include "xwin_sdl.h"
#include "utils.h"
#include "menu_parameters.h"
#include "computation.h"

int active_rect_id = EMPTY_ID;
bool quit_after = false;

// Setup buttons in menu
button_t c_re = { .id = C_RE_ID, .x_start = X_FIRST_COL, .x_end = X_FIRST_COL + BOX_WIDTH, .y_start = Y_FIRST_ROW, .y_end = Y_FIRST_ROW + BOX_HEIGHT };
button_t c_im = { .id = C_IM_ID, .x_start = X_SEC_COL, .x_end = X_SEC_COL + BOX_WIDTH, .y_start = Y_FIRST_ROW, .y_end = Y_FIRST_ROW + BOX_HEIGHT };

button_t re_min = { .id = RE_MIN_ID, .x_start = X_FIRST_COL, .x_end = X_FIRST_COL + BOX_WIDTH, .y_start = Y_SEC_ROW, .y_end = Y_SEC_ROW + BOX_HEIGHT };
button_t re_max = { .id = RE_MAX_ID, .x_start = X_SEC_COL, .x_end = X_SEC_COL + BOX_WIDTH, .y_start = Y_SEC_ROW, .y_end = Y_SEC_ROW + BOX_HEIGHT };

button_t im_min = { .id = IM_MIN_ID, .x_start = X_FIRST_COL, .x_end = X_FIRST_COL + BOX_WIDTH, .y_start = Y_THIRD_ROW, .y_end = Y_THIRD_ROW + BOX_HEIGHT };
button_t im_max = { .id = IM_MAX_ID, .x_start = X_SEC_COL, .x_end = X_SEC_COL + BOX_WIDTH, .y_start = Y_THIRD_ROW, .y_end = Y_THIRD_ROW + BOX_HEIGHT };

button_t n = { .id = N_ID, .x_start = X_FIRST_COL, .x_end = X_FIRST_COL + BOX_WIDTH, .y_start = Y_FOURTH_ROW, .y_end = Y_FOURTH_ROW + BOX_HEIGHT };
button_t save = { .id = SAVE_ID, .x_start = X_SEC_COL, .x_end = X_SEC_COL + BOX_WIDTH, .y_start = Y_FOURTH_ROW, .y_end = Y_FOURTH_ROW + BOX_HEIGHT };

button_t message_but = { .id = MESSAGE_ID, .x_start = X_FIRST_COL, .x_end = X_FIRST_COL + BOX_WIDTH, .y_start = Y_FIFTH_ROW + BOX_HEIGHT, .y_end = Y_FIFTH_ROW + 2*BOX_HEIGHT };


void handle_menu_event(SDL_Event event, int *menu_id) {
  if (event.type == SDL_KEYUP) {
    switch(event.key.keysym.sym) {
      case SDLK_q:
        if (*menu_id != 0) {
          menu_close();
          *menu_id = 0;
          active_rect_id = EMPTY_ID;
        }
        break;
    }
  } else if ((event.type == SDL_WINDOWEVENT &&event.window.event == SDL_WINDOWEVENT_CLOSE)
              || quit_after) {
    if (*menu_id != 0) {
      menu_close();
      *menu_id = 0;
      active_rect_id = EMPTY_ID;
      quit_after = false;
    }
  } else if (event.type == SDL_MOUSEBUTTONUP) {
    if (is_button(&save, event.button.x, event.button.y)) {
      save_new_constants();
      active_rect_id = EMPTY_ID;
      info("Variables saved");
      create_menu(get_menu_renderer(), false, SUCCESS_SAVE);


    } else {
      set_active_id(event.button.x, event.button.y);
      handle_text_input();
    }
  }
}

void create_menu(SDL_Renderer *menu_renderer, bool load_constants, char *error_mess) {
  TTF_Init();
  TTF_Font *text_font = TTF_OpenFont(FONT_PATH, TEXT_FONT_SIZE);
  TTF_Font *input_font = TTF_OpenFont(FONT_PATH, INPUT_FONT_SIZE);

  if (text_font == NULL || input_font == NULL) {
    error("Font not found!");
    return;
  }

  SDL_Color text_color = { 255, 255, 255, 255 };
  SDL_Color error_color = { 255, 0, 0, 255 }; 
  SDL_Color success_color = { 0, 255, 0, 255 };

  if (load_constants) {
    sprintf(c_re.value, "%.3f", get_c_re());
    sprintf(c_im.value, "%.3f", get_c_im());

    sprintf(re_min.value, "%.3f", get_range_re_min());
    sprintf(re_max.value, "%.3f", get_range_re_max());
    sprintf(im_min.value, "%.3f", get_range_im_min());
    sprintf(im_max.value, "%.3f", get_range_im_max());

    sprintf(n.value, "%d", get_n());
  }

  SDL_SetRenderDrawColor(menu_renderer, 0,0,0,255);

  SDL_RenderClear(menu_renderer);
  create_menu_part(menu_renderer, &c_re, text_font, input_font, TEXT_C_RE, &text_color);
  create_menu_part(menu_renderer, &c_im, text_font, input_font, TEXT_C_IM, &text_color);

  create_menu_part(menu_renderer, &re_min, text_font, input_font, TEXT_RE_MIN, &text_color);
  create_menu_part(menu_renderer, &re_max, text_font, input_font, TEXT_RE_MAX, &text_color);

  create_menu_part(menu_renderer, &im_min, text_font, input_font, TEXT_IM_MIN, &text_color);
  create_menu_part(menu_renderer, &im_max, text_font, input_font, TEXT_IM_MAX, &text_color);

  create_menu_part(menu_renderer, &n, text_font, input_font, TEXT_N, &text_color);

  create_box_with_text(menu_renderer, &save, TEXT_SAVE, text_font, &text_color, true);

  if (error_mess) {
    if (!strcmp(error_mess, WRONG_INPUT_MESS)) {
      create_box_with_text(menu_renderer, &message_but, error_mess, text_font, &error_color, false);
    } else if (!strcmp(error_mess, SUCCESS_INPUT) || !strcmp(error_mess, SUCCESS_SAVE)) {
      create_box_with_text(menu_renderer, &message_but, error_mess, text_font, &success_color, false);
    }
  }

  SDL_RenderPresent(menu_renderer);

  TTF_CloseFont(text_font);
  TTF_CloseFont(input_font);
  TTF_Quit();
}

void create_menu_part(SDL_Renderer *renderer, button_t *button, TTF_Font *text_font, TTF_Font *input_font, char *text, SDL_Color *text_color)
{
  create_box_with_text(renderer, button, text, text_font, text_color, false);
  create_box_with_text(renderer, button, button->value, input_font, text_color, true);
}

void create_box_with_text(SDL_Renderer *renderer, button_t *button, char *text, TTF_Font *font, SDL_Color *text_color, bool input)
{
  SDL_Surface *surface = TTF_RenderText_Blended(font, text, *text_color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect rect = { .x = button->x_start, .y = button->y_start, .w = surface->w, .h = surface->h};

  if (input)
    rect.y += BOX_HEIGHT;

  SDL_FreeSurface(surface);

  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  SDL_RenderCopy(renderer, texture, NULL, &rect);

  if (input) {
    SDL_Rect outline_rect = { .x = button->x_start, .y = button->y_start + BOX_HEIGHT, .w = button->x_end - button->x_start, .h = button->y_end - button->y_start};
    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    SDL_RenderDrawRect(renderer, &outline_rect);
  }

  SDL_DestroyTexture(texture);
}

void handle_text_input()
{
  if (active_rect_id == EMPTY_ID)
    return;

  bool done = false;
  SDL_Event ev;
  int i = 0;

  bool dot = false;
  unsigned short int is_minus = 0;
  char *error_mess = NULL;

  button_t *button = get_button_from_id();
  fprintf(stderr, "This is the button clicked: %d\n", button->id);

  for (int j = 0; j <= INPUT_LEN; j++)
    button->value[j] = ' ';

  create_menu(get_menu_renderer(), false, NULL);

  while(!done) {
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_TEXTINPUT) {
        if (ASCII_ZERO <= ev.text.text[0] && ev.text.text[0] <= ASCII_NINE) {
          button->value[i++] = ev.text.text[0];
          error_mess = NULL;

        } else if (ev.text.text[0] == ASCII_DOT && dot == false) {
          button->value[i++] = ev.text.text[0];
          dot = true;
          error_mess = NULL;

        } else if (ev.text.text[0] == ASCII_MINUS && i == 0) {
          button->value[i++] = ev.text.text[0];
          is_minus = 1;
          error_mess = NULL;

        } else {
          error_mess = WRONG_INPUT_MESS;
        }
        create_menu(get_menu_renderer(), false, error_mess);
      } else if (ev.type == SDL_MOUSEBUTTONDOWN)
        break;
      else if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE) {
        quit_after = true;
        break;
      }
    }
    if (i >= INPUT_LEN + is_minus) {
      done = true;
      debug("done");
      create_menu(get_menu_renderer(), false, SUCCESS_INPUT);
    }
  }
}

bool is_button(button_t *button, int x, int y)
{
  if (button->x_start < x && x < button->x_end
    && button->y_start + BOX_HEIGHT < y && y < button->y_end + BOX_HEIGHT)
    return true;
  else
    return false;
}

void set_active_id(int x, int y)
{
  if (is_button(&c_re, x, y))
    active_rect_id = c_re.id;
  else if (is_button(&c_im, x, y))
    active_rect_id = c_im.id;
  else if (is_button(&re_min, x, y))
    active_rect_id = re_min.id;
  else if (is_button(&re_max, x, y))
    active_rect_id = re_max.id;
  else if (is_button(&im_min, x, y))
    active_rect_id = im_min.id;
  else if (is_button(&im_max, x, y))
    active_rect_id = im_max.id;
  else if (is_button(&n, x, y))
    active_rect_id = n.id;
  else 
    active_rect_id = EMPTY_ID;
}

button_t *get_button_from_id()
{
  if (c_re.id == active_rect_id)
    return &c_re;
  else if (c_im.id == active_rect_id)
    return &c_im;
  else if (re_min.id == active_rect_id)
    return &re_min;
  else if (re_max.id == active_rect_id)
    return &re_max;
  else if (im_min.id == active_rect_id)
    return &im_min;
  else if (im_max.id == active_rect_id)
    return &im_max;
  else
    return &n;
}

void save_new_constants()
{
  set_range_re_min(atof(re_min.value));
  set_range_re_max(atof(re_max.value));
  set_range_im_min(atof(im_min.value));
  set_range_im_max(atof(im_max.value));

  set_c_re(atof(c_re.value));
  set_c_im(atof(c_im.value));

  set_n(atoi(n.value));
  computation_init();
}
