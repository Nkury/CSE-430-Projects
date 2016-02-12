#include <linux/syscalls.h>
#include <linux/kernel.h> /* Needed for KERN_INFO*/
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/tty.h>
#include <linux/jiffies.h>
#include "ProcessInfo.h"

asmlinkage long sys_my_syscall(int a, int b, int* pids, int* times, char &comm[][16], char &name[][64]){

	int taskPIDs[10000];
	int taskTimes[10000];
	char taskComms[10000][16];
	char taskNames[10000][64];

	int count = 0;
	struct task_struct *task;	// instantiate an instance of task from task_struct that will be traversed in the following loop

	// we loop through each of the tasks
	for_each_process(task){
		taskPIDs[count] = task->pid;
		long userTime = task->utime / HZ; // convert user time (in jiffies) to seconds
		long execTime = task->stime / HZ; // convert exec time (in jiffies) to seconds
		taskTimes[count] = userTime + execTime; // sum of user and exec time is process time
		taskComms[count] = task->comm;
		taskNames[count] = task->signal->tty->name;
		count++;
	}

	int a = copy_to_user(pids, taskPIDS, sizeof(int)); // use copy_to_user to actually copy kernel info to user space
	int b = copy_to_user(times, taskTimes, sizeof(int)); // use copy_to_user to actually copy kernel info to user space
	int c = copy_to_user(comm, taskComms, sizeof(taskComms)); // use copy_to_user to actually copy kernel info to user space
	int d = copy_to_user(name, taskNames, sizeof(taskNames)); // use copy_to_user to actually copy kernel info to user space
	return a + b + c + d; // returns 0 if copied successfully
}