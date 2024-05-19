/*
 * filename: gui.h
 * date: 2024/05/01 08:50 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#ifndef __GUI_H__
#define __GUI_H__

#define ARROW_UP 1073741906
#define ARROW_DOWN 1073741905
#define SDL_EVENT_POLL_WAIT_MS 10

void gui_init(void);
void gui_cleanup(void);

void gui_refresh(void);
void gui_resize(int w, int h);
int round_pixels(int pixel);
void generate_menu_button(void);

void* gui_win_thread(void*);

#endif

/* end of gui.h */
