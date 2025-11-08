CC=gcc
CFLAGS=-I.

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: consumer_busy_waiting_semaphore

consumer_busy_waiting_semaphore: consumer_busy_waiting_semaphore.o
	$(CC) -o consumer_busy_waiting_semaphore consumer_busy_waiting_semaphore.o -pthread

clean:
	rm -f *.o consumer_busy_waiting_semaphore

.PHONY: all clean