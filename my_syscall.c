#include <linux/syscalls.h>
#include <linux/kernel.h> /* Needed for KERN_INFO*/
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/tty.h>
#include <linux/jiffies.h>
#include "ProcessInfo.h"

asmlinkage long sys_my_syscall(int a, int b, process_info* list){

	// i may need to use malloc? I'm not sure on that front
	struct process_info buffer[1024]; // define the buffer that will communicate info between user-space and kernel-space
	int count = 0;
	struct task_struct *task;	// instantiate an instance of task from task_struct that will be traversed in the following loop

	// we loop through each of the tasks
	for_each_process(task){
		struct process_info instance; // for storing information of a current task/process
		instance.pid = task->pid;
		long userTime = task->utime / HZ; // convert user time (in jiffies) to seconds
		long execTime = task->stime / HZ; // convert exec time (in jiffies) to seconds
		instance.time_in_seconds = userTime + execTime; // sum of user and exec time is process time
		instance.comm = task->comm;
		instance.name = task->signal->tty->name;
		buffer[count] = instance;
		count++;
	}

	int n = copy_to_user(list, buffer, sizeof(process_info)); // use copy_to_user to actually copy kernel info to user space
	return n; // returns 0 if copied successfully
}