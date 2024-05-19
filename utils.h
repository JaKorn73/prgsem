/*
 * filename: utils.h
 * date: 2024/04/27 14:39 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
 * inspired by: Jan Faigl
*/

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include <stdlib.h>

void my_assert(bool r, const char *fcname, int line, const char *fname);
void *my_alloc(size_t size);
void *my_calloc(size_t size);
void call_termios(int reset);
void info(const char *str);
void debug(const char *str);
void error(const char *str);
void warn(const char *str);

#endif

/* end of utils.h */
