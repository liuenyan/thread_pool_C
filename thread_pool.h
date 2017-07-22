#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "thread_worker.h"

/*线程池管理器结构定义*/
typedef struct {
    /*当前线程池中的线程数*/
    int thread_count;
    /*线程ID信息*/
    thread_worker_t *workers;
} thread_pool_t;

thread_pool_t *thread_pool_create(int thread_count);
int thread_pool_add_task(thread_pool_t *tp, \
        task_routine_t routine, void *routine_args);

#endif /* ifndef THREAD_POOL_H*/
