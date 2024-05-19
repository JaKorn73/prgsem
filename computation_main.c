/*
 * filename: computation_module.c
 * date: 2024/05/19 10:28
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "computation_main.h"
#include "utils.h"
#include "fractals.h"

void *main_thread(void *d)
{
  my_assert(d != NULL, __func__, __LINE__, __FILE__) ;
  int pipe_out = *(int*)d;
  message msg;
  uint8_t msg_buf[sizeof(message)];
  int msg_len;
  bool quit = false;

  do {
    event ev = queue_pop();
    msg.type = MSG_NBR;
    switch(ev.type) {
      case EV_PIPE_IN_MESSAGE:
        process_pipe_message(&ev, pipe_out);
        break;

      case EV_DONE:
        msg.type = MSG_DONE;
        break;

      case EV_QUIT:
        set_quit();
        break;

      case EV_GET_VERSION:
        msg.type = MSG_VERSION;
        msg.data.version.major = 3;
        msg.data.version.minor = 1;
        msg.data.version.patch = 0;
        break;

      case EV_COMPUTE:
        msg.type = MSG_OK;
        break;
      
      case EV_ABORT:
        msg.type = MSG_OK;
        break;

      default:
        break;
    }
    if (msg.type != MSG_NBR) {
      my_assert(fill_message_buf(&msg, msg_buf, sizeof(message), &msg_len), __func__, __LINE__, __FILE__);
      if (write(pipe_out, msg_buf, msg_len) != msg_len)
        error("Message failed");
    }
    quit = is_quit();
  } while(!quit);

  return NULL;
}

void process_pipe_message(event* const ev, int pipe_out)
{
  my_assert(ev != NULL && ev->type == EV_PIPE_IN_MESSAGE && ev->data.msg, __func__, __LINE__, __FILE__);
  ev->type = EV_TYPE_NUM;
  const message *msg = ev->data.msg;
  event new_ev;

  switch (msg->type) {
    case MSG_STARTUP:
      fprintf(stderr, "STARTUP recieved: %s\n", msg->data.startup.message);
      break;

    case MSG_QUIT:
      new_ev.type = EV_QUIT;
      set_quit();
      queue_push(new_ev);
      break;

    case MSG_GET_VERSION:
      new_ev.type = EV_GET_VERSION;
      queue_push(new_ev);
      break;

    case MSG_SET_COMPUTE:
      set_fractal_comp(msg);
      break;

    case MSG_COMPUTE:
      if (!is_aborted()) {
        new_ev.type = EV_COMPUTE;
        queue_push(new_ev);
        compute_fractal(msg, pipe_out);
        new_ev.type = EV_DONE;
        queue_push(new_ev);

      }
      break;

    case MSG_ABORT:
      new_ev.type = EV_ABORT;
      queue_push(new_ev);
      break;

    default:
      fprintf(stderr, "Unhandled pipe message type %d\n", msg->type);
      break;
  } //switch end

  free(ev->data.msg);
  ev->data.msg = NULL;
}
