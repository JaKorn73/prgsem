/*
 * filename: menu.c
 * date: 2024/05/14 11:04 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "menu.h"
#include "gui.h"
#include "xwin_sdl.h"
#include "utils.h"
#include "menu_parameters.h"
#include "computation.h"

int active_rect_id = -1;

void create_box_with_text(SDL_Renderer *renderer, TTF_Font *font, char *text, SDL_Color *text_color, int text_x_start, int text_y_start, bool outline)
{
  SDL_Surface *surface = TTF_RenderText_Blended(font, text, *text_color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect rect = { .x = text_x_start, .y = text_y_start, .w = surface->w, .h = surface->h};
  
  SDL_FreeSurface(surface);

  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  SDL_RenderCopy(renderer, texture, NULL, &rect);

  if (outline) {
    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    SDL_RenderDrawRect(renderer, &rect);
  }

  SDL_DestroyTexture(texture);
}

void create_menu_part(SDL_Renderer *renderer, TTF_Font *text_font, TTF_Font *input_font, char *text, char *input_text, SDL_Color *text_color, int text_x_start, int text_y_start)
{
  create_box_with_text(renderer, text_font, text, text_color, text_x_start, text_y_start, false);
  create_box_with_text(renderer, input_font, input_text, text_color, text_x_start, text_y_start + TEXT_FONT_SIZE + PADDING, true);
}

void create_menu(SDL_Window *menu, SDL_Renderer *menu_renderer) {
  TTF_Init();
  TTF_Font *text_font = TTF_OpenFont(FONT_PATH, TEXT_FONT_SIZE);
  TTF_Font *input_font = TTF_OpenFont(FONT_PATH, INPUT_FONT_SIZE);

  if (text_font == NULL || input_font == NULL) {
    error("Font not found!");
    return;
  }

  SDL_Color text_color = { 255, 255, 255, 255 };  

  char range_re_min_str[10];
  char range_re_max_str[10];
  char range_im_min_str[10];
  char range_im_max_str[10];

  char c_re[10];
  char c_im[10];
  char n_str[10];

  sprintf(range_re_min_str, "%.2f", get_range_re_min());
  sprintf(range_re_max_str, "%.2f", get_range_re_max());
  sprintf(range_im_min_str, "%.2f", get_range_im_min());
  sprintf(range_im_max_str, "%.2f", get_range_im_max());

  sprintf(c_re, "%.2f", get_c_re());
  sprintf(c_im, "%.2f", get_c_im());

  sprintf(n_str, "%d", get_n());

  SDL_SetRenderDrawColor(menu_renderer, 0,0,0,255);

  SDL_RenderClear(menu_renderer);
  create_menu_part(menu_renderer, text_font, input_font, TEXT_C_RE, c_re, &text_color, X_FIRST_COL, Y_FIRST_ROW);
  create_menu_part(menu_renderer, text_font, input_font, TEXT_C_IM, c_im, &text_color, X_SEC_COL, Y_FIRST_ROW);

  create_menu_part(menu_renderer, text_font, input_font, TEXT_RE_MIN, range_re_min_str, &text_color, X_FIRST_COL, Y_SEC_ROW);
  create_menu_part(menu_renderer, text_font, input_font, TEXT_RE_MAX, range_re_max_str, &text_color, X_SEC_COL, Y_SEC_ROW);

  create_menu_part(menu_renderer, text_font, input_font, TEXT_IM_MIN, range_im_min_str, &text_color, X_FIRST_COL, Y_THIRD_ROW);
  create_menu_part(menu_renderer, text_font, input_font, TEXT_IM_MAX, range_im_max_str, &text_color, X_SEC_COL, Y_THIRD_ROW);

  create_menu_part(menu_renderer, text_font, input_font, TEXT_N, n_str, &text_color, X_FIRST_COL, Y_FOURTH_ROW);

  SDL_RenderPresent(menu_renderer);
  TTF_Quit();
}


void handle_menu_event(SDL_Event event, int *menu_id) {
  if (event.type == SDL_KEYDOWN) {
    switch(event.key.keysym.sym) {
      case SDLK_q:
        if (*menu_id != 0) {
          menu_close();
          *menu_id = 0;
          active_rect_id = -1;
        }
        break;
    }
  } else if (event.type == SDL_WINDOWEVENT) {
    if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
      if (*menu_id != 0) {
        menu_close();
        *menu_id = 0;
        active_rect_id = -1;
      }
    }
  } else if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.x > X_FIRST_COL && event.button.x < X_FIRST_COL + BOX_WIDTH && event.button.y > Y_FIRST_ROW + TEXT_FONT_SIZE + PADDING && event.button.y < Y_FIRST_ROW + TEXT_FONT_SIZE + PADDING + BOX_HEIGHT) {
      debug("First rectangle is active");
      active_rect_id = 0;
    } else if (true) {

    }

    debug("Mousebuttonup");
    // handle_text_input();
  } else if (event.type == SDL_TEXTINPUT) {
    debug("Text input!!!");
    handle_text_input(event);
    
  }
}

void handle_text_input(SDL_Event event)
{
  if (active_rect_id == -1)
    return;
  // SDL_bool done = SDL_FALSE;
  // char text[10];
  // sprintf(text, "%.2f", get_range_re_min());
  // int text_length = 4;

  // while(!done) {
  //   SDL_Event event;
  //   if (SDL_PollEvent(&event)) {
  //     switch(event.type) {
  //       case SDL_QUIT:
  //         done = SDL_TRUE;
  //         break;
  //       case SDL_TEXTINPUT:
  //         fprintf(stderr, "text: %s\n", event.text.text);
  //         fprintf(stderr, "text: %d\n", event.text.type);

  //         strcat(text, event.text.text);

  //         break;
  //       case SDL_TEXTEDITING:
  //         fprintf(stderr, "AAAAA\n");
  //         break;

  //       case SDL_KEYDOWN:
  //         fprintf(stderr, "keydown: %d\n", event.key.keysym.sym);
  //         // if (event.key.keysym.sym == 8) {

  //         // }
  //         break;

  //       default:
  //         break;
  //     }
  //   }
  // }
  // SDL_StopTextInput();
  // fprintf(stderr, "This is the text: %s\n", text);
  // free(text);
}
