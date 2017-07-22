#include "thread_worker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*工作线程函数*/
void *worker_thread(void *args)
{
    thread_worker_t *tw = (thread_worker_t *)args;
    task_routine_t routine;
    void *routine_args;
    
    for(;;) {
        /* 等待条件变量发生 */
        pthread_mutex_lock(&(tw->worker_mutex));
        while(task_queue_empty(&tw->queue)) {
            pthread_cond_wait(&(tw->worker_cond), &(tw->worker_mutex));
        }

        /*获取一个任务*/
        task_queue_remove_head(&tw->queue, &routine, &routine_args);
        pthread_mutex_unlock(&tw->worker_mutex);

        /* 执行任务 */
        routine(routine_args);
        /* 释放参数内存 */
        free(routine_args);
    }
    return NULL;
}

/*创建一个工作线程*/
pthread_t thread_worker_init(thread_worker_t *tw)
{
    task_queue_init(&tw->queue);
    int value = pthread_mutex_init(&(tw->worker_mutex), NULL); 
    if(value != 0) {
        fprintf(stderr, "create thread_mutex failed: %s\n", strerror(value));
        exit(EXIT_FAILURE);
    }
    printf("create thread_mutex successfully\n");

    value = pthread_cond_init(&(tw->worker_cond), NULL); 
    if(value != 0) {
        fprintf(stderr, "create thread_mutex failed: %s\n", strerror(value));
        exit(EXIT_FAILURE);
    }
    printf("create thread_cond successfully\n");
    
    value = pthread_create(&(tw->tid), NULL, worker_thread, tw); 
    if(value != 0) {
        fprintf(stderr, "create thread failed: %s\n", strerror(value));
        exit(EXIT_FAILURE);
    }
    printf("create thread %ld successfully\n", tw->tid);

    return tw->tid;
}
