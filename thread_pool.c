#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ERR_EXIT(msg) do{perror(msg); exit(EXIT_FAILURE); } while(0)

/*创建一个线程池*/
thread_pool_t *thread_pool_create(int thread_count)
{
    thread_pool_t *tp = (thread_pool_t *)malloc(sizeof(thread_pool_t));
    if(!tp)
        ERR_EXIT("malloc");
    
    tp->thread_count = thread_count;
    
    tp->workers = (thread_worker_t *)calloc(thread_count, sizeof(thread_worker_t));
    if(!tp->workers)
        ERR_EXIT("calloc");

    for(int i=0; i<thread_count; i++){
        thread_worker_init(&tp->workers[i]);
    }
    return tp;
}

/*向线程池中添加任务*/
int thread_pool_add_task(thread_pool_t *tp, \
        task_routine_t routine, void *routine_args)
{
    int i;
    for(i=0; i<tp->thread_count; i++) {
        thread_worker_t *worker = &tp->workers[i];
        pthread_mutex_lock(&worker->worker_mutex);
        if(task_queue_full(&worker->queue)) {
            pthread_mutex_unlock(&worker->worker_mutex);
            continue;
        }
        
        task_queue_insert_tail(&worker->queue, routine, routine_args);
        pthread_cond_signal(&worker->worker_cond);
        pthread_mutex_unlock(&worker->worker_mutex);
        return 0;
    }
    return -1;
}
