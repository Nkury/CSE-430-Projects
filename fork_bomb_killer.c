#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/sched.h>		/* Needed for 'for_each_process' */
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/signal.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/semaphore.h>

struct semaphore fork_sem;

struct task_struct *task;
struct task_struct *detector; // detects fork bombs
struct task_struct *scanner; // scans processes for new process entries

struct list_head *clearP;
struct list_head *clearQ;

int count = 0;
int currentProcessCount = 0;
int threshold = 50;

struct processTable {
	char name[64];
	int pCount;
	struct list_head list;
};

struct processTable myTable;
struct processTable *tempProc;

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

void kill_process(char* target);

int my_kthread_function(void* data){

	INIT_LIST_HEAD(&myTable.list);
	while (!kthread_should_stop()){

		down_interruptible(&fork_sem);
			struct list_head *p;
			struct processTable *temp;
			struct processTable *ts;
			for_each_process(task){
				if (task->pid != 1){
					if (!checkName(task->comm)){
						list_for_each(p, &(myTable.list)){
							ts = list_entry(p, struct processTable, list);
							if (strcmp(ts->name, task->comm) == 0){
								ts->pCount++;
							}
						}
					}
					else{
						temp = kmalloc(sizeof(struct processTable), GFP_KERNEL);
						strcpy(temp->name, task->comm);
						temp->pCount = 1;
						list_add(&(temp->list), &(myTable.list));
					}
				}
			}

			// checks to see if it can kill processes after a second
			list_for_each(p, &(myTable.list)){
				ts = list_entry(p, struct processTable, list);
				if (ts->pCount > threshold){
					kill_process(ts->name);
				}
			}

			list_for_each(p, &(myTable.list)){
				ts = list_entry(p, struct processTable, list);
				ts->pCount = 0;
			}
	}
	return 0;
}

int scan_function(void *data){
	while (!kthread_should_stop()){
		struct task_struct *cTask; // for iterating tasks
		for_each_process(cTask){
			count++;
		}

		if (count != currentProcessCount){
			currentProcessCount = count;
			up(&fork_sem);
		}

		count = 0;
	}
}

// takes parameter name of target. Goes through all the tasks using
// for_each_process and eliminates all processes that have the target
// name

void kill_process(char* target){

	for_each_process(task) {
		if (strcmp(task->comm, target) == 0) {
			printk("KILLED: %s %d\n", task->comm, task->pid);
			send_sig(SIGKILL, task, 0);
		}
	}
}

static int __init fork_bomb_killer(void){
	int data = 20;

	// initialize semaphore
	sema_init(&fork_sem, 0);

	// We can instantiate multiple threads, but I think one should suffice?
	detector = kthread_run(
		&my_kthread_function,
		(void*)data,
		"my_kthread");
	scanner = kthread_run(
		&scan_function,
		(void*)data,
		"scan_kthread");
	return 0;
}

static void __exit fork_bomb_killer_exit(void){
	// clears the process table memory
	list_for_each_safe(clearP, clearQ, &myTable.list){
		tempProc = list_entry(clearP, struct processTable, list);
		list_del(clearP);
		kfree(tempProc);
	}
	kthread_stop(task);
}

module_init(fork_bomb_killer);
module_exit(fork_bomb_killer_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Fork Bomb Defuser");
