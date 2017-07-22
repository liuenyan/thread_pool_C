#ifndef THREAD_POOL_H
#define THREAD_POOL_H



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
