/*
 * filename: menu.c
 * date: 2024/05/14 11:04 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#include <SDL.h>
#include <SDL_ttf.h>

#include "menu.h"
#include "gui.h"
#include "xwin_sdl.h"
#include "utils.h"

#define FONT_PATH "./fonts/Roboto-Black.ttf"
#define FONT_SIZE 30
#define MENU_TEXT "Kornejak10"
// #define TEXT_WIDTH 50
// #define TEXT_HEIGHT 20
  

void create_menu(SDL_Window *menu, SDL_Renderer *menu_renderer) {
  TTF_Init();
  TTF_Font *font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
  if (font == NULL) {
    error("Font not found");
    return;
  }


  SDL_Color text_color = { 255, 255, 255, 255 };
  SDL_Surface *surface = TTF_RenderText_Solid(font, MENU_TEXT, text_color);
  // SDL_Texture *texture = SDL_CreateTextureFromSurface(menu_renderer, surface);
  fprintf(stderr, "SDL_ERROR: %s\n", surface == NULL ? "Texture is null" : "");
  // int text_width = surface->w;
  // int text_height = surface->h;




  // SDL_Rect rect1;
  // rect1.x = 0;
  // rect1.y = 0;
  // rect1.w = text_width;
  // rect1.h = text_height;

  TTF_Quit();
  SDL_FreeSurface(surface);
  // SDL_DestroyTexture(texture);
  // TTF_Quit();

  SDL_SetRenderDrawColor(menu_renderer, 255,0,0,255);
  SDL_RenderClear(menu_renderer);

  SDL_RenderPresent(menu_renderer);


  // SDL_DestroyTexture(texture);
  // SDL_FreeSurface(surface);
  // TTF_Quit();
  // my_assert(SDL_RenderClear(menu_renderer) == 0, __func__, __LINE__, __FILE__);
  // my_assert(SDL_RenderCopy(menu_renderer, texture, NULL, &rect1) == 0, __func__, __LINE__, __FILE__);

  // // SDL_RenderPresent(menu_renderer);
  // SDL_RenderClear(menu_renderer);
  // debug("After render");
  // SDL_Delay(100);



  
}


void handle_menu_event(SDL_Event event, int *menu_id) {
  if (event.type == SDL_KEYDOWN) {
    switch(event.key.keysym.sym) {
      case SDLK_q:
        if (*menu_id != 0) {
          menu_close();
          *menu_id = 0;
        }
        break;
  }
  } else if (event.type == SDL_WINDOWEVENT) {
    if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
      if (*menu_id != 0) {
        menu_close();
        *menu_id = 0;
      }
    }
  }
}
