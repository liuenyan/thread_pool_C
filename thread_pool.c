#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include "thread_worker.h"

#define ERR_EXIT(msg) do{perror(msg); exit(EXIT_FAILURE); } while(0)

thread_pool_t *thread_pool_create(int min, int max)
{
    thread_pool_t *tp = (thread_pool_t *)malloc(sizeof(thread_pool_t));
    if(!tp)
        ERR_EXIT("malloc");
    
    tp->thread_min = min;
    tp->thread_max =max;
    tp->workers = (thread_worker_t *)calloc(max, sizeof(thread_worker_t));
    if(!tp->workers)
        ERR_EXIT("calloc");

    return tp;
}
