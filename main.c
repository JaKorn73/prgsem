/*
 * filename: main.h
 * date: 2024/04/29 08:33 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
 * inspired by: Jan Faigl
*/

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "main.h"

#include "utils.h"
#include "event_queue.h"
#include "messages.h"
#include "computation.h"
#include "gui.h"

void* main_thread(void* d)
{
  my_assert(d != NULL, __func__, __LINE__, __FILE__) ;
  int pipe_out = *(int*)d;
  message msg;
  uint8_t msg_buf[sizeof(message)];
  int msg_len;
  bool quit = false;

  computation_init();
  gui_init();

  while(!quit) {
    event ev = queue_pop();
    msg.type = MSG_NBR;
    switch(ev.type) {
      case EV_STARTUP:
        msg.type = MSG_STARTUP;
        strcpy(msg.data.startup.message,"kornejak");
        break;

      case EV_QUIT:
        debug("Quit received");
        msg.type = MSG_QUIT;
        break;

      case EV_GET_VERSION:
        msg.type = MSG_GET_VERSION;
        break;

      case EV_SET_COMPUTE:
        info(set_compute(&msg) ? "set compute" : "fail set compute");
        break;

      case EV_COMPUTE:
        if (is_abort())
          sleep((unsigned int)0.1);
        enable_comp();
        info(compute(&msg) ? "compute" : "fail compute");
        break;
      
      case EV_COMPUTE_CPU:
        enable_comp();
        compute_background();
        info(compute(&msg) ? "compute" : "fail compute");
        break;
      
      case EV_CLEAR_BUFFER:
        clean_buffer();
        break;

      case EV_REFRESH:
        reset_cid();
        break;

      case EV_SHOW:
        gui_refresh();
        break;

      case EV_ABORT:
        abort_comp();
        msg.type = MSG_ABORT;
        break;

      case EV_PIPE_IN_MESSAGE:
        process_pipe_message(&ev);
        break;

      default:
        break;
    }
    if (msg.type != MSG_NBR) {
      my_assert(fill_message_buf(&msg, msg_buf, sizeof(message), &msg_len), __func__, __LINE__, __FILE__);
      if (write(pipe_out, msg_buf, msg_len) != msg_len)
        error("send message fail!");
    }
    quit = is_quit();
  }
    
  gui_cleanup();
  computation_cleanup();

  return NULL;
}

void process_pipe_message(event *const ev)
{
  my_assert(ev != NULL && ev->type == EV_PIPE_IN_MESSAGE && ev->data.msg, __func__, __LINE__, __FILE__);
  ev->type = EV_TYPE_NUM;
  const message *msg = ev->data.msg;

  switch (msg->type) {
    case MSG_OK:
      info("OK");
      break;

    case MSG_VERSION:
      fprintf(stderr, "INFO: Module version %d.%d-p%d\n", msg->data.version.major, msg->data.version.minor, msg->data.version.patch);
      break;

    case MSG_COMPUTE_DATA:
      update_data(&(msg->data.compute_data));
      break;

    case MSG_DONE:
      if (!is_abort()) {
        if (!is_background()) {
          gui_refresh();
        }
        if (is_done())
          info("Computation done");
        else {
          event ev = { .type = EV_COMPUTE };
          queue_push(ev);
        }
      }
      break;

    default:
      fprintf(stderr, "Unhandled pipe message type %d\n", msg->type);
      break;
  } //switch end
  
  free(ev->data.msg);
  ev->data.msg = NULL;
}
