#include <linux/syscalls.h>
#include <linux/kernel.h> /* Needed for KERN_INFO*/
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/tty.h>
#include <linux/jiffies.h>
#include <string>
#include <stdio.h>

asmlinkage long sys_my_syscall(int index, char* buffer){

	char buff[1024];

	int count = 0;
	int buffLength;
	struct task_struct *task;	// instantiate an instance of task from task_struct that will be traversed in the following loop

	// we loop through each of the tasks
	for_each_process(task){
		if (count == index){
			int id = task->pid;
			long userTime = task->utime / HZ; // convert user time (in jiffies) to seconds
			long execTime = task->stime / HZ; // convert exec time (in jiffies) to seconds
			int time = userTime + execTime; // sum of user and exec time is process time
			char taskComm[16] = task->comm;
			char taskName[64] = task->signal->tty->name;
			sprintf(buff, "%d %s %d %s", id, taskName, time, taskComm); // stores the values in the buffer
			buffLength = strlen(buff); // gets the size of the array
		}
		count++;
	}

	int x = copy_to_user(buffer, buff, buffLength); // use copy_to_user to actually copy kernel info to user space
	return x; // returns 0 if copied successfully
}