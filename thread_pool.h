#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include "task_queue.h"

typedef enum {
    WORKER_FREE,
    WORKER_TASK_ADD,
    WORKER_BUSY,
} worker_status_t;

/*工作线程任务队列*/
typedef struct {
    void *args;
    void *(*task_function)(void *args);
} thread_task_t;

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

/*线程池管理器结构定义*/
typedef struct {
    /*最多线程数*/
    int thread_max;
    /*最少线程数*/
    int thread_min;
    /*当前线程池中的线程数*/
    int thread_count;
    /*线程ID信息*/
    thread_worker_t *workers;
} thread_pool_t;

thread_pool_t *thread_pool_create(int min, int max);

#endif /* ifndef THREAD_POOL_H*/
