/*
 * filename: event_queue.c
 * date: 2024/04/27 15:07 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
 * inspired by: Jan Faigl
*/

#ifndef __EVENT_QUEUE_H__
#define __EVENT_QUEUE_H__

#include "messages.h"

typedef enum {
   EV_STARTUP,
   EV_COMPUTE, // request compute on nucleo with particular 
   EV_RESET_CHUNK, //reset the chunk id
   EV_ABORT,
   EV_GET_VERSION,
   EV_THREAD_EXIT,
   EV_QUIT,
   EV_SET_COMPUTE,
   EV_COMPUTE_CPU,
   EV_CLEAR_BUFFER,
   EV_REFRESH,
   EV_PIPE_IN_MESSAGE,
   EV_DONE,
   EV_SHOW,
   EV_TYPE_NUM
} event_type;

typedef struct {
   int param;
} event_keyboard;


typedef struct {
   event_type type;
   union {
      int param;
      message *msg;
   } data;
} event;

void queue_init(void);

void queue_cleanup(void);

event queue_pop(void);

void queue_push(event ev);

bool is_quit(void);

void set_quit(void);

#endif

/* end of event_queue.h */
