/*
 * filename: computation_module.c
 * date: 2024/05/11 17:17 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#ifndef __COMPUTATION_MAIN__
#define __COMPUTATION_MAIN__

#include "event_queue.h"

void *main_thread(void *d);
void process_pipe_message(event* const ev, int pipe_out);

#endif

/* end of computation_main.h */
