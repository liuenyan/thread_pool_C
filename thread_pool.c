#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define ERR_EXIT(msg) do{perror(msg); exit(EXIT_FAILURE); } while(0)

/*工作线程函数*/
static void *worker_thread(void *args)
{
    thread_pool_t *tp = (thread_pool_t *)args;
    pthread_mutex_t *mutex = &tp->mutex;
    pthread_cond_t *cond = &tp->cond;
    struct task_queue *queue = &tp->queue;

    task_routine_t routine;
    void *routine_args;
    
    for(;;) {
        /* 等待条件变量发生 */
        pthread_mutex_lock(mutex);
        while(task_queue_empty(queue)) {
            pthread_cond_wait(cond, mutex);
        }

        /*获取一个任务*/
        task_queue_remove_head(queue, &routine, &routine_args);
        pthread_mutex_unlock(mutex);

        /* 执行任务 */
        routine(routine_args);
        /* 释放参数内存 */
        free(routine_args);
    }
    return NULL;
}

/*创建一个工作线程*/
static pthread_t thread_worker_init(thread_pool_t *tp, int index)
{
    int value = pthread_create(&(tp->worker_tids[index]), NULL, 
            worker_thread, (void *)tp);
    if(value != 0) {
        fprintf(stderr, "create thread failed: %s\n", strerror(value));
        exit(EXIT_FAILURE);
    }
    printf("create thread %ld successfully\n", tp->worker_tids[index]);
    return tp->worker_tids[index];
}

/*创建一个线程池*/
thread_pool_t *thread_pool_create(int thread_count)
{
    thread_pool_t *tp = (thread_pool_t *)malloc(sizeof(thread_pool_t));
    if(!tp)
        ERR_EXIT("malloc");
    
    /*创建线程池互斥量*/
    int value = pthread_mutex_init(&(tp->mutex), NULL); 
    if(value != 0) {
        fprintf(stderr, "create thread_pool mutex failed: %s\n", strerror(value));
        exit(EXIT_FAILURE);
    }
    printf("create thread_pool mutex successfully\n");
    
    /*线程池条件变量*/
    value = pthread_cond_init(&(tp->cond), NULL); 
    if(value != 0) {
        fprintf(stderr, "create thread_pool cond failed: %s\n", strerror(value));
        exit(EXIT_FAILURE);
    }
    printf("create thread_pool cond successfully\n");
    
    task_queue_init(&tp->queue);
    
    tp->thread_count = thread_count;
    
    /*申请线程信息存储空间*/
    tp->worker_tids = (pthread_t *)calloc(thread_count, sizeof(pthread_t));
    if(!tp->worker_tids)
        ERR_EXIT("calloc");

    for(int i=0; i<thread_count; i++){
        thread_worker_init(tp, i);
    }
    return tp;
}

/*向线程池中添加任务*/
int thread_pool_add_task(thread_pool_t *tp, \
        task_routine_t routine, void *routine_args)
{
    int i;
    for(i=0; i<tp->thread_count; i++) {
        pthread_mutex_lock(&tp->mutex);
        if(task_queue_full(&tp->queue)) {
            pthread_mutex_unlock(&tp->mutex);
            continue;
        }
        
        task_queue_insert_tail(&tp->queue, routine, routine_args);
        pthread_cond_signal(&tp->cond);
        pthread_mutex_unlock(&tp->mutex);
        return 0;
    }
    return -1;
}

