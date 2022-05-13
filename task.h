#ifndef __OL113_TASK_H__
#define __OL113_TASK_H__

#include <time.h> /* time_t */
#include "uid.h" /* unique_id_t */

typedef int (*oper_function_t)(void *param);
typedef struct task task_t;

task_t *TaskCreate(oper_function_t oper_func, void *param, size_t interval);
void TaskDestroy(task_t *task);

unique_id_t TaskGetUID(const task_t *task);
time_t TaskGetNextRunTime(const task_t *task);

int TaskRun(task_t *task);
void TaskUpdateNextRunTime(task_t *task);

int TaskCompareNextRunTime(const void *task1, const void *task2);

#endif /*__OL113_TASK_H__ */

