#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include "task_queue.h"

/*线程池管理器结构定义*/
typedef struct {
    /*当前线程池中的线程数*/
    int thread_count;
    /*线程ID信息*/
    pthread_t *worker_tids;
    /*线程池互斥锁*/
    pthread_mutex_t mutex;
    /*线程池条件变量*/
    pthread_cond_t cond;
    /*线程池任务队列*/
    struct task_queue queue;
} thread_pool_t;

thread_pool_t *thread_pool_create(int thread_count);

int thread_pool_destroy(thread_pool_t *tp);

int thread_pool_add_task(thread_pool_t *tp, \
        task_routine_t routine, void *routine_args);

#endif /* ifndef THREAD_POOL_H*/
