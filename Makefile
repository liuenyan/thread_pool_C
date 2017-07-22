all: thread_pool_test
CFLAGS=-g -Wall
LIBS=-lpthread

thread_pool_test: thread_pool.o task_queue.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	- $(RM) *.o thread_pool_test


