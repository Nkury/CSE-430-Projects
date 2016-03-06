#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/sched.h>		/* Needed for 'for_each_process' */
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/signal.h>
#include <linux/kthread.h>
#include <linux/slab.h>

struct task_struct *task;

int count = 0;
int threshold = 100;

struct processTable {
	char name[64];
	int pCount;
	struct list_head list;
};

struct processTable myTable;

bool checkName(char* name){
	struct list_head *pos;
	struct processTable *temp;
	list_for_each(pos, &(myTable.list)){
		temp = list_entry(pos, struct processTable, list);
		if (strcmp(temp->name, name) == 0){
			return false;
		}
	}
	return true;
}

int my_kthread_function(void* data){

	INIT_LIST_HEAD(&myTable.list);
	while (!kthread_should_stop()){
		struct list_head *p;
		struct processTable *temp;
		struct processTable *ts;
		for_each_process(task){
			if (task->pid != 1){
				if (checkName(task->comm)){
					list_for_each(p, &(myTable.list)){
						ts = list_entry(p, struct processTable, list);
						if (strcmp(ts->name, task->comm)){
							ts->pCount++;
						}
					}
				}
				else{
					temp = kmalloc(sizeof(struct processTable), GFP_KERNEL);
					strcpy(temp->name, task->comm);
					temp->pCount = 0;
					list_add(&(temp->list), &(myTable.list));
					// kfree(temp) not sure to put this in here
				}
			}
		}
		// checks to see if it can kill processes after a second
		oom_kill_process(p);
		msleep(1000);
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
