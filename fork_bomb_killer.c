#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/sched.h>		/* Needed for 'for_each_process' */
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/signal.h>

struct task_struct *task;
int count = 0;
int threshold = 100;

void countChildren(struct_task* traverse){
	struct list_head *p;
	struct task_struct ts;
	
	if (traverse == null){
		return;
	}
	else{
		list_for_each(p, &(traverse)){
			ts = *list_entry(p, struct task_struct, sibling);
			countChildren(ts.children);
			count++;
		}
	}
}

void countChildren(struct_task* traverse){
	struct list_head *p;
	struct task_struct ts;

	if (traverse == null){
		return;
	}
	else{
		list_for_each(p, &(traverse)){
			ts = *list_entry(p, struct task_struct, sibling);
			countChildren(ts.children);
			printk(KERN_INFO "%d\n", ts.pid);
		}
	}
}

int my_kthread_function(void* data){
	while (!kthread_should_stop()){
		// most of the time kthread sleeps
		msleep(1000);
		//read_lock(&tasklist_lock); // lock the task_struct
		struct list_head *p;
		struct task_struct ts;
		for_each_process(task){
			if (task->pid != 1){
				countChildren(task);
				if (count > threshold){
					printk(KERN_INFO "FORK BOMB DETECTED! Here are the processes\n");
					printk(KERN_INFO "%d\n", task->pid);
					printChildren(task);
				}
			}
			count = 0;
		}
		//read_unlock(&tasklist_lock);
		//** check Grant's data structure for depth greater than 3
		//** call Jackson's kill command (WIP)
		oom_kill_process(p);
	}
	return 0;
}

void oom_kill_process (void* victim){
	send_sig_info(SIGKILL, SEND_SIG_FORCED, victim);
	
	for_each_process(task) {
		if (!process_shares_mm(task, mm))
			continue;
		else
			send_sig_info(SIGKILL, SEND_SIG_FORCED, task);
	}
}

static int __init fork_bomb_killer(void){
	int data;
	data = 20;

	// We can instantiate multiple threads, but I think one should suffice?
	task = kthread_run(
		&my_kthread_function,
		(void*)data,
		"my_kthread");

	return 0;
}

static void __exit fork_bomb_killer_exit(void){
	kthread_stop(task);
}

module_init(fork_bomb_killer);
module_exit(fork_bomb_killer_exit);
