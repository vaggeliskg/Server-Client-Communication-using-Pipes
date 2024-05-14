CC = gcc
CFLAGS = -Wall -pthread

all: jobExecutorServer jobCommander

jobExecutorServer: jobExecutorServer.c issueJob.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

jobCommander: jobCommander.c queue.o
	$(CC) $(CFLAGS) -o $@ $^

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c -o $@ queue.c

issueJob.o: issueJob.c 
	$(CC) $(CFLAGS) -c -o $@ issueJob.c

clean:
	rm -f jobExecutorServer jobCommander myfifo answer *.o

.PHONY: all makeclean
