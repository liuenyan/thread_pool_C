#include <stdio.h>
#include <stdlib.h>
#include "thread_worker.h"


void *print_func(void *args)
{
    printf("count is %d\n", *(int *)args);
    return NULL;
}

int main()
{

    thread_worker_t tw;
    pthread_t tid = thread_worker_init(&tw);
    void *ret;

    int i = 0;
    for(;;) {
        pthread_mutex_lock(&tw.worker_mutex);
        if(task_queue_full(&tw.queue)) {
            pthread_mutex_unlock(&tw.worker_mutex);
            continue;
        }

        int *args = (int *)malloc(sizeof(int));
        *args = i++;

        task_queue_insert_tail(&tw.queue, print_func, (void *)args);
        pthread_cond_signal(&tw.worker_cond);
        pthread_mutex_unlock(&tw.worker_mutex);
        //usleep(10);
    }
    pthread_join(tid, &ret);
    return 0;

}
