/*
 * filename: gui.h
 * date: 2024/05/01 08:50 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#ifndef __GUI_H__
#define __GUI_H__

void gui_init(void);
void gui_cleanup(void);

void gui_refresh(void);
void generate_menu_button(void);

void* gui_win_thread(void*);
#endif