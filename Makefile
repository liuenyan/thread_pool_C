all: thread_pool_test
CFLAGS=-g -Wall
LIBS=-lpthread

thread_pool_test: test.o task_queue.o thread_worker.o thread_pool.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	- $(RM) *.o thread_pool_test
