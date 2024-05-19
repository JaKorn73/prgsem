/*
 * filename: computation_module.c
 * date: 2024/05/11 17:17 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#include "utils.h"
#include "prg_io_nonblock.h"
#include "computation_main.h"

#ifndef IO_READ_TIMEOUT_MS
#define IO_READ_TIMEOUT_MS 100
#endif

void *read_pipe_thread(void* d);

int main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;
	const char *fname_pipe_in = argc > 1 ? argv[1] : "/tmp/computational_module.in";
	const char *fname_pipe_out = argc > 2 ? argv[2] : "/tmp/computational_module.out";

	int pipe_in = io_open_read(fname_pipe_in);
	int pipe_out = io_open_write(fname_pipe_out);

	my_assert(pipe_in != -1 && pipe_out != -1, __func__, __LINE__, __FILE__);

	enum { READ_PIPE_THREAD, COMP_THREAD, NUM_THREADS};
	const char *thread_names[] = {"ReadPipe", "Computation"};
	void* (*thread_functions[])(void*) = { read_pipe_thread, main_thread };
	pthread_t threads[NUM_THREADS];

	void* thread_data[NUM_THREADS] = {};
	thread_data[READ_PIPE_THREAD] = &pipe_in;
	thread_data[COMP_THREAD] = &pipe_out;

	for (int i = 0; i < NUM_THREADS; ++i) {
		int r = pthread_create(&threads[i], NULL, thread_functions[i], thread_data[i]);
		printf("Create thread '%s' %s\r\n", thread_names[i], ( r == 0 ? "OK" : "FAIL") );
	}

  int *ex;
	for (int i = 0; i < NUM_THREADS; ++i) {
		printf("Call join to the thread %s\r\n", thread_names[i]);
		int r = pthread_join(threads[i], (void*)&ex);
		printf("Joining the thread %s has been %s\r\n", thread_names[i], (r == 0 ? "OK" : "FAIL"));
	}
  
	io_close(pipe_in);
	io_close(pipe_out);

	return ret;
}

void* read_pipe_thread(void* d)
{
	my_assert(d != NULL, __func__, __LINE__, __FILE__);

	int pipe_in = *(int*)d;
	bool end = false;
	int i = 0;
	int len = 0;
	uint8_t msg_buf[sizeof(message)];

	unsigned char c;
	while(io_getc_timeout(pipe_in, IO_READ_TIMEOUT_MS, &c) > 0) {}; // clean pipe

  while(!end) {
    int r = io_getc_timeout(pipe_in, IO_READ_TIMEOUT_MS, &c);
    if (r > 0) { // char has been read
      if (i == 0) {
        if (get_message_size(c, &len))
          msg_buf[i++] = c;
        else
          fprintf(stderr, "ERROR: unknown message type %d", c);
      } else { //read remaining bytes of the message
        msg_buf[i++] = c;
      }
		if (len > 0 && i == len) {
			message *msg = my_alloc(sizeof(message));
			if (parse_message_buf(msg_buf, len, msg)) {
				event ev = { .type = EV_PIPE_IN_MESSAGE };
				ev.data.msg = msg;
				queue_push(ev);
			} else {
				fprintf(stderr, "ERROR: cannot parse message");
				free(msg);
			}
			i = len = 0;
		}
    } else if (r == 0) { // timeout

    } else { //handle error
      fprintf(stderr, "ERROR: reading from pipe\n");
      set_quit();
      event ev = { .type = EV_QUIT };
      queue_push(ev);
    }
	  end = is_quit();
  }
	return NULL;
}
