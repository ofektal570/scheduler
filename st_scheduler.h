#ifndef __OL113_ST_SCHEDULER_H__
#define __OL113_ST_SCHEDULER_H__

#include <stddef.h> /* size_t */
#include "uid.h" /* unique_id_t */

/*****************************************************************************
 * version 3                                                                 *
 *****************************************************************************/

typedef int (*operation_function_t)(void *param);
typedef struct st_scheduler st_scheduler_t;

st_scheduler_t *STSchedulerCreate(void);
void STSchedulerDestory(st_scheduler_t *scheduler);

unique_id_t STSchedulerAdd(
	st_scheduler_t *scheduler,
	operation_function_t action,
	size_t sec_interval,
	void *param
);

void STSchedulerRemove(st_scheduler_t *scheduler, unique_id_t task_id);
void STSchedulerClear(st_scheduler_t *scheduler);

size_t STSchedulerSize(const st_scheduler_t *scheduler);
int STSchedulerIsEmpty(const st_scheduler_t *scheduler);

void STSchedulerRun(st_scheduler_t *scheduler);

/* How to use:
 * write a function that calls Stop(scheduler) and add
 * add it to the scheduler using Add(), with the
 * scheduler itlself as the second parameter
 */
void STSchedulerStop(st_scheduler_t *scheduler);

#endif /*__OL113_ST_SCHEDULER_H__ */
