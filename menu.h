/*
 * filename: menu.h
 * date: 2024/05/14 11:05 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#ifndef __MENU_H__
#define __MENU_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

typedef struct clickable_rectangle {
    unsigned int id;
    int x_start;
    int x_end;
    int y_start;
    int y_end;
    char value[10];
} button_t;

void handle_menu_event(SDL_Event event, int *menu_id);
void create_menu(SDL_Renderer *menu_renderer, bool load_constants, char *error_mess);
void create_box_with_text(SDL_Renderer *renderer, button_t *button, char *text, TTF_Font *font, SDL_Color *text_color, bool input);
void create_menu_part(SDL_Renderer *renderer, button_t *button, TTF_Font *text_font, TTF_Font *input_font, char *text, SDL_Color *text_color);
void handle_text_input();

bool is_button(button_t *button, int x, int y);
void set_active_id(int x, int y);
button_t *get_button_from_id();
void save_new_constants();

#endif

/* end of menu.h */
