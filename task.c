#include <assert.h> /* assert () */
#include <stdlib.h> /* malloc(), free() */
#include "task.h"

struct task
{
	oper_function_t op_func;
	unique_id_t uid;
	void *param;
	size_t interval;
	time_t next_run_time;	
};

task_t *TaskCreate(oper_function_t oper_func, void *param, size_t interval)
{
	task_t *task = NULL;
	
	assert(oper_func);
	
	task = malloc(sizeof(task_t));
	
	if (!task)
	{
		return NULL;
	}
	
	task->op_func = oper_func;
	task->uid = UidCreate();
	task->param = param;
	task->interval = interval;
	task->next_run_time = time(NULL);
	
	return task;
}

void TaskDestroy(task_t *task)
{
	assert(task);

	free(task);
	task = NULL;
}

unique_id_t TaskGetUID(const task_t *task)
{
	assert(task);

	return task->uid;
}

time_t TaskGetNextRunTime(const task_t *task)
{
	assert(task);

	return task->next_run_time;
}

int TaskRun(task_t *task)
{
	assert(task);

	return task->op_func(task->param);
}

void TaskUpdateNextRunTime(task_t *task)
{
	assert(task);
	
	task->next_run_time += task->interval;
}

int TaskCompareNextRunTime(const void *task1, const void *task2)
{
	task_t *p_task1 = (task_t *)task1;
	task_t *p_task2 = (task_t *)task2;	
	
	assert(task1 && task2);
	
	return (int)(p_task1->next_run_time - p_task2->next_run_time);
}
