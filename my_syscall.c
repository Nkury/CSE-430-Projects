#include <linux/syscalls.h>
#include <linux/kernel.h> /* Needed for KERN_INFO*/
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/tty.h>
#include <linux/jiffies.h>
#include <linux/linkage.h>

asmlinkage long sys_my_syscall(int index, char* buffer){


	char buff[800];
	int id, x;
	cputime_t utime, stime, jiff;
	char taskName[64];
	char taskComm[64];
	int secs, minutes, hours;
	char timeFormat[9];
	int count = 0;
	int buffLength = 0;

	struct task_struct *task;	// instantiate an instance of task from task_struct that will be traversed in the following loop

	// we loop through each of the tasks
	for_each_process(task){
		if (count == index){
			id = task->pid;
			utime = 0;
			stime = 0;
			thread_group_cputime_adjusted(task, &utime, &stime);
			jiff = utime + stime;
			secs = jiff / HZ;
			hours = secs / 3600;
			secs = secs - hours * 3600;
			minutes = secs / 60;
			secs = secs - minutes * 60;
			snprintf(timeFormat, sizeof(timeFormat), "%.2d:%.2d:%.2d", (int)hours, (int)minutes, (int)secs);


			/*
			userTime = task->utime / HZ; // convert user time (in jiffies) to seconds
			execTime = task->stime / HZ; // convert exec time (in jiffies) to seconds
			time = userTime + execTime; // sum of user and exec time is process time
			*/
			strcpy(taskComm, task->comm);
			if (task->signal->tty == NULL){
				strcpy(taskName, "?");
			}
			else{
				strcpy(taskName, task->signal->tty->name);
			}
			snprintf(buff, sizeof(buff), "%5d %-8s %8s %s", id, taskName, timeFormat, taskComm); // stores the values in the buffer
			buffLength = strlen(buff); // gets the size of the array
			if (buffLength > 800)
				buffLength = 800;
		}
		count++;
	}

	x = copy_to_user(buffer, buff, buffLength); // use copy_to_user to actually copy kernel info to user space
	return count; // returns 0 if copied successfully
}

