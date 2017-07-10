CC = gcc

SOURCES =	aio.c
OBJS = aio.o

CFLAGS = -Wall -std=c99

.PHONY: all test build clean

all: test

test: build
	gcc -o aio_test aio_test.c aio.o
	./aio_test

build: $(OBJS)

%.o: %.c
	@echo GENERATE $<
	@ $(CC) $(CFLAGS) -c $<

clean:
	@echo CLEANING
	@ $(RM) $(OBJS)
