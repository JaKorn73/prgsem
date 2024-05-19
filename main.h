/*
 * filename: main.h
 * date: 2024/04/29 08:33 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
 * inspired by: Jan Faigl
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#include "event_queue.h"

void *main_thread(void*);
void process_pipe_message(event* const ev);

#endif

/* end of main.h */
