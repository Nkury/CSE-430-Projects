#include <linux/syscalls.h>
#include <linux/kernel.h> /* Needed for KERN_INFO*/
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/tty.h>
#include <linux/jiffies.h>
#include <linux/linkage.h>

asmlinkage long sys_my_syscall(int index, char* buffer){


	char buff[1024];
	int id, time, x;
	long userTime, execTime;
	char taskName[64];
	char taskComm[64];

	int count = 0;
	int buffLength = 0;
	struct task_struct *task;	// instantiate an instance of task from task_struct that will be traversed in the following loop

	// we loop through each of the tasks
	for_each_process(task){
		if (count == index){
			id = task->pid;
			userTime = task->utime / HZ; // convert user time (in jiffies) to seconds
			execTime = task->stime / HZ; // convert exec time (in jiffies) to seconds
			time = userTime + execTime; // sum of user and exec time is process time
			strcpy(taskComm, task->comm);
			strcpy(taskName, task->signal->tty->name);
			sprintf(buff, "%d %s %d %s", id, taskName, time, taskComm); // stores the values in the buffer
			buffLength = strlen(buff); // gets the size of the array
			if (buffLength > 1024)
				buffLength = 1024;
		}
		count++;
	}

	x = copy_to_user(buffer, buff, buffLength); // use copy_to_user to actually copy kernel info to user space
	return x; // returns 0 if copied successfully
}

