/*
 * Filename: xwin_sdl.h
 * Date:     2015/06/18 14:37
 * Author:   Jan Faigl
 * Modified by: Jakub Kornel
 */

#ifndef __XWIN_SDL_H__
#define __XWIN_SDL_H__

int xwin_init(int w, int h);
int menu_init();

void xwin_close();
void menu_close();

void xwin_redraw(int w, int h, unsigned char *img);
void xwin_poll_events(void);

void *get_menu_renderer();
void *get_menu_win();

#endif

/* end of xwin_sdl.h */
