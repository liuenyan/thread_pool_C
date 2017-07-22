#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread_pool.h"

void *print_func(void *args) {
    printf("count is %d\n", *(int *)args);
    return NULL;
}

int main()
{
    thread_pool_t *tp = thread_pool_create(4);

    int i = 0;
    for(;;) {

        int *args = (int *)malloc(sizeof(int));
        *args = i;

        while(thread_pool_add_task(tp, print_func, args) == -1) {
            printf("add %d failed.\n", i);
            usleep(1);
        }
        i++;
    }
    return 0;
}
