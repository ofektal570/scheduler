#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert () */
#include <unistd.h> /* sleep () */
#include "st_scheduler.h"
#include "sorted_dl_list.h"
#include "task.h"

struct st_scheduler
{
	sorted_list_t *sorted_list;
	int keep_running;
};

st_scheduler_t *STSchedulerCreate(void)
{
	st_scheduler_t *scheduler = malloc(sizeof(st_scheduler_t));

	if (!scheduler)
	{
		return NULL;
	}

	scheduler->sorted_list = SortedListCreate(TaskCompareNextRunTime);
	scheduler->keep_running = 0;

	if (!scheduler->sorted_list)
	{
		free(scheduler);
		scheduler = NULL;

		return NULL;
	}

	return scheduler;
}

void STSchedulerDestory(st_scheduler_t *scheduler)
{
	assert(scheduler);

	STSchedulerClear(scheduler);

	SortedListDestroy(scheduler->sorted_list);

	free(scheduler);
	scheduler = NULL;
}

unique_id_t STSchedulerAdd(
	st_scheduler_t *scheduler,
	operation_function_t action,
	size_t sec_interval,
	void *param)
{
	task_t *new_task = NULL;

	assert(scheduler && action);

	new_task = TaskCreate(action, param, sec_interval);

	if (!new_task)
	{
		return BadUID;
	}

	if (NULL == (SortedListInsert(scheduler->sorted_list, new_task)).iter)
	{
		return BadUID;
	}

	return TaskGetUID(new_task);
}

static int IsSameUID(const void *current, const void *uid_to_find)
{
	return UidIsMatch(TaskGetUID((task_t *)current), *(unique_id_t *)uid_to_find);
}

void STSchedulerRemove(st_scheduler_t *scheduler, unique_id_t task_id)
{
	sorted_list_iter_t found_iter = {0};
	sorted_list_iter_t end_iter = SortedListEnd(scheduler->sorted_list);

	assert(scheduler);

	found_iter = SortedListFindIf(SortedListBegin(scheduler->sorted_list),
								  end_iter, &task_id, IsSameUID);

	if (!SortedListIsSameIterator(found_iter, end_iter))
	{
		TaskDestroy(SortedListGetData(found_iter));
		SortedListRemove(found_iter);
	}
}

void STSchedulerClear(st_scheduler_t *scheduler)
{
	assert(scheduler);

	while (!SortedListIsEmpty(scheduler->sorted_list))
	{
		TaskDestroy(SortedListPopFront(scheduler->sorted_list));
	}
}

size_t STSchedulerSize(const st_scheduler_t *scheduler)
{
	assert(scheduler);

	return SortedListSize(scheduler->sorted_list);
}

int STSchedulerIsEmpty(const st_scheduler_t *scheduler)
{
	assert(scheduler);

	return SortedListIsEmpty(scheduler->sorted_list);
}

void STSchedulerRun(st_scheduler_t *scheduler)
{
	assert(scheduler);

	scheduler->keep_running = 1;

	while (!STSchedulerIsEmpty(scheduler) && scheduler->keep_running)
	{
		int ret_from_action = 0;
		task_t *curr_task = (task_t *)SortedListPopFront(scheduler->sorted_list);
		time_t when_to_run = TaskGetNextRunTime(curr_task);
		time_t curr_time = time(NULL);
		size_t sec_to_sleep = when_to_run > curr_time ? when_to_run - curr_time : 0;

		while (sleep(sec_to_sleep))
			;

		ret_from_action = TaskRun(curr_task);
		TaskUpdateNextRunTime(curr_task);

		if (ret_from_action)
		{
			TaskDestroy(curr_task);
			curr_task = NULL;
		}
		else
		{
			SortedListInsert(scheduler->sorted_list, curr_task);
		}
	}
}

void STSchedulerStop(st_scheduler_t *scheduler)
{
	assert(scheduler);

	scheduler->keep_running = 0;
}
