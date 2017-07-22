#include "task_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * 初始化任务队列
 */
int task_queue_init(struct task_queue *q)
{
    q->count = 0;
    /*将队列指向首个节点的指针置为 NULL*/
    struct task_queue_head *q_head = &q->q_head;
    q_head->stqh_first = NULL;
    q_head->stqh_last = &(q_head->stqh_first);
    return 0;
}

/** 
 * 向任务队列添加一条任务, 成功返回 0, 失败返回 -1
 */
int task_queue_insert_tail(struct task_queue *q, 
        task_routine_t routine, void *args) {
    
    struct task_queue_entry *entry = \
        (struct task_queue_entry *)malloc(sizeof(struct task_queue_entry));
    if(!entry) {
        return -1;
    }
    entry->routine = routine;
    entry->args = args;

    STAILQ_INSERT_TAIL(&(q->q_head), entry, entries);

    q->count++;
    return 0;
}

/**
 * 从任务队列里获取一条任务, 成功返回 0, 失败返回 -1
 */
int task_queue_remove_head(struct task_queue *q, 
        task_routine_t *routine, void **args)
{
    struct task_queue_head *q_head = &q->q_head;
    if(STAILQ_EMPTY(q_head)) {
        return -1;
    }
    struct task_queue_entry *entry = STAILQ_FIRST(q_head);
    *routine = entry->routine;
    *args = entry->args;
    STAILQ_REMOVE_HEAD(q_head, entries);
    free(entry);

    q->count--;
    return 0;
}
