CC = gcc
CFLAGS = -Wall -pthread

all: jobExecutorServer jobCommander

jobExecutorServer: jobExecutorServer.c queue.o
	$(CC) $(CFLAGS) -o $@ $^

jobCommander: jobCommander.c queue.o
	$(CC) $(CFLAGS) -o $@ $^

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c -o $@ queue.c


clean:
	rm -f jobExecutorServer jobCommander myfifo *.o

.PHONY: all makeclean
