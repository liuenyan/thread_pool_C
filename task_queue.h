#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <sys/queue.h>

#define QUEUE_COUNT_MAX 128

typedef void *(*task_routine_t)(void *args);

/*任务队列定义*/
struct task_queue_entry {
    /*任务参数*/
    void *args;
    /*任务执行函数*/
    task_routine_t routine;
    /*指向下一个元素的指针*/
    STAILQ_ENTRY(task_queue_entry) entries;
};

/*队列头部 struct task_queue_head 定义*/
STAILQ_HEAD(task_queue_head, task_queue_entry);

struct task_queue {
    /*队列头部*/
    struct task_queue_head q_head;
    /*当前队列中任务的数量*/
    int count;
};

#define task_queue_empty(q) STAILQ_EMPTY(&((q)->q_head))
#define task_queue_full(q) ((q)->count == QUEUE_COUNT_MAX)

/*队列初始化函数*/
int task_queue_init(struct task_queue *q);

/*队列入队函数*/
int task_queue_insert_tail(struct task_queue *q, 
        task_routine_t routine, void *args);

/*队列出队函数*/
int task_queue_remove_head(struct task_queue *q, 
        task_routine_t *routine, void **args);

/*队列销毁函数*/
int task_queue_destroy(struct task_queue *q);

#endif /* ifndef TASK_QUEUE_H */
