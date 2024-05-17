/*
 * filename: menu.h
 * date: 2024/05/14 11:05 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#include <SDL.h>

void handle_menu_event(SDL_Event event, int *menu_id);
void create_menu(SDL_Window *menu, SDL_Renderer *menu_renderer);
