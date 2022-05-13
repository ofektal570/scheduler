#include <stdio.h>  /* printf() */
#include <stdlib.h> /* system() */
#include <assert.h> /* assert() */

#include "st_scheduler.h"

void Red(void)
{
	printf("\033[1;31m");
}

void Green(void) 
{
	printf("\033[1;32m");
}

void Yellow(void)
{
	printf("\033[33m");
}

void White(void)
{
	printf("\033[1m\033[37m");
}

void Reset(void) 
{
	printf("\033[0m");
}

void PrintSucceeded(void)
{
	Green();
	printf("The Test Succeeded!!\n");
	Reset();
}

void PrintFailed(int ans, size_t line)
{
	Red();
	printf("\n\nThe Test Failed!!\n");
	printf("Error in line:%ld\nThe ans is %d\n\n", line, ans);
	Reset();
}

void IsExpectedEqualAns(int expected, int ans, char *str, size_t line)
{
	printf("Testing %s :\n", str);
	if (expected == ans)
	{
		PrintSucceeded();
	}
	else
	{
		PrintFailed(ans, line);
	}
}

void TestIsEmptyAndSize(int expected_empty, int is_empty,
						 size_t expected_size, size_t size, size_t line)
{
	IsExpectedEqualAns(expected_empty, is_empty, "Is Empty", line);
	IsExpectedEqualAns(expected_size, size, "Size", line);
}

void PrintAction(char *str)
{
	Yellow();
	printf("\n\n\t\t%s\n\n", str);
	Reset();	
}

int PrintStr(void *str)
{
    static int i = 0;
	
	White();
    printf("%s\n", (char *)str);
    Reset();
    
    i++;
    
    if (i < 3)
    {
        return 0;
    }

    return 1;
}

int StopFunc(void *scheduler)
{
    static int j = 0;  
      
    if (j)
    {
    	STSchedulerStop(scheduler);
    	
        return 1;
    }
    
	j++;
    
    return 0;
}

void TestStSchedularRemove(void)
{
    st_scheduler_t *new_scheduler = STSchedulerCreate();
    unique_id_t uid1 = {0};
    
    PrintAction("TestStSchedularRemove");
    PrintAction("Creating scheduler...");
    TestIsEmptyAndSize(1, STSchedulerIsEmpty(new_scheduler), 0
    				   , STSchedulerSize(new_scheduler), __LINE__);
    				   
    PrintAction("Adding Func1 to scheduler"); 
	STSchedulerAdd(new_scheduler, PrintStr, 1, "Every 1 seconds");
    TestIsEmptyAndSize(0, STSchedulerIsEmpty(new_scheduler), 1
    				   , STSchedulerSize(new_scheduler), __LINE__);
    				   
    PrintAction("Adding Func2 to scheduler");
	uid1 = STSchedulerAdd(new_scheduler, PrintStr, 2, "Every 2 second");
	IsExpectedEqualAns(2, STSchedulerSize(new_scheduler), "Size", __LINE__);
	
	PrintAction("Adding Func3 to scheduler");
	STSchedulerAdd(new_scheduler, PrintStr, 3, "Every 3 second");
	IsExpectedEqualAns(3, STSchedulerSize(new_scheduler), "Size", __LINE__);
	
	PrintAction("Remove Func2 from scheduler");
	STSchedulerRemove(new_scheduler, uid1);
	IsExpectedEqualAns(2, STSchedulerSize(new_scheduler), "Size", __LINE__);

    STSchedulerRun(new_scheduler);

    STSchedulerDestory(new_scheduler);
    new_scheduler = NULL;
}

void TestStSchedularClear(void)
{
    st_scheduler_t *new_scheduler = STSchedulerCreate();
    
    PrintAction("TestStSchedularClear");
    TestIsEmptyAndSize(1, STSchedulerIsEmpty(new_scheduler), 0
    				   , STSchedulerSize(new_scheduler), __LINE__);
    				   
    PrintAction("Adding Func1 to scheduler"); 
	STSchedulerAdd(new_scheduler, PrintStr, 1, "Every 1 seconds");
    TestIsEmptyAndSize(0, STSchedulerIsEmpty(new_scheduler), 1
    				   , STSchedulerSize(new_scheduler), __LINE__);
    	   
    PrintAction("Adding Func2 to scheduler");
	STSchedulerAdd(new_scheduler, PrintStr, 2, "Every 2 second");
	IsExpectedEqualAns(2, STSchedulerSize(new_scheduler), "Size", __LINE__);
	
	PrintAction("Adding Func3 to scheduler");
	STSchedulerAdd(new_scheduler, PrintStr, 3, "Every 3 second");
	IsExpectedEqualAns(3, STSchedulerSize(new_scheduler), "Size", __LINE__);
	
	PrintAction("Cleaninig scheduler..");
	STSchedulerClear(new_scheduler);
    TestIsEmptyAndSize(1, STSchedulerIsEmpty(new_scheduler), 0
    				   , STSchedulerSize(new_scheduler), __LINE__);
    				   	
    STSchedulerDestory(new_scheduler);
    new_scheduler = NULL;
}

void TestStSchedularStop(void)
{
    st_scheduler_t *new_scheduler = STSchedulerCreate();
    
    PrintAction("TestStSchedularStop");
    				   
    PrintAction("Adding Func1, Func2, Func3 to scheduler"); 
	STSchedulerAdd(new_scheduler, PrintStr, 1, "Every 1 seconds");
    STSchedulerAdd(new_scheduler, PrintStr, 2, "Every 2 seconds");
    STSchedulerAdd(new_scheduler, PrintStr, 3, "Every 3 seconds");
    TestIsEmptyAndSize(0, STSchedulerIsEmpty(new_scheduler), 3
    				   , STSchedulerSize(new_scheduler), __LINE__);	   
    PrintAction("Adding StopFunc to scheduler after 3 sec");
    STSchedulerAdd(new_scheduler, StopFunc, 3, new_scheduler);
    
    STSchedulerRun(new_scheduler);
     				   	
    STSchedulerDestory(new_scheduler);
    new_scheduler = NULL;
}

int main()
{
    TestStSchedularRemove();
	TestStSchedularClear();
	TestStSchedularStop();
		
    return 0;
}








