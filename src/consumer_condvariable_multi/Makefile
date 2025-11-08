CC=gcc
CFLAGS=-I.

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: consumer_condvariable_multi_semaphore

consumer_condvariable_multi_semaphore: consumer_condvariable_multi_semaphore.o
	$(CC) -o consumer_condvariable_multi_semaphore consumer_condvariable_multi_semaphore.o -pthread

clean:
	rm -f *.o consumer_condvariable_multi_semaphore

.PHONY: all clean