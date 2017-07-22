#ifndef THREAD_WORKER
#define THREAD_WORKER

#include <pthread.h>
#include "task_queue.h"

/*工作线程结构体定义*/
typedef struct {
    /*线程 ID*/
    pthread_t tid;
    /*线程互斥锁*/
    pthread_mutex_t worker_mutex;
    /*线程条件变量*/
    pthread_cond_t worker_cond;
    /*线程任务队列*/
    struct task_queue queue;
} thread_worker_t;

pthread_t thread_worker_init(thread_worker_t *tw);
void *worker_thread(void *args);

#endif /* ifndef THREAD_WORKER */
