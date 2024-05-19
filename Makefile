CC := clang
CFLAGS+= -Wall -Werror -std=c99 -g
LDFLAGS=-lpthread -lm

BINARIES=comp prgsem

CFLAGS+=$(shell sdl2-config --cflags)
LDFLAGS+=$(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf

all: $(BINARIES)

ALL_FILES = $(wildcard *.c)

OBJS=$(patsubst %.c,%.o,$(ALL_FILES))
OBJS_COMP_MODULE=$(patsubst %.c,%.o,$(filter-out prgsem.c main.c, $(ALL_FILES)))
OBJS_PRGSEM=$(patsubst %.c,%.o,$(filter-out computation_main.c computation_module.c fractals.c, $(ALL_FILES)))


prgsem: $(OBJS_PRGSEM)
	$(CC) $(OBJS_PRGSEM) $(LDFLAGS) -o $@

comp: $(OBJS_COMP_MODULE)
	$(CC) $(OBJS_COMP_MODULE) $(LDFLAGS) -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BINARIES) $(OBJS)
