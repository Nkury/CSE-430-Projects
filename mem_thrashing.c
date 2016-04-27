#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/sched.h>		/* Needed for 'for_each_process' */
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/signal.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/linkage.h>
#include <linux/highmem.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/rmap.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <asm/page.h>
#include <linux/rwsem.h>
#include <linux/hugetlb.h>
#include <asm/pgtable_types.h>
#include <asm/pgtable.h>
#include <asm/tlbflush.h>

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

void kill_process(char* target);
int my_kthread_function(void* data){

	INIT_LIST_HEAD(&myTable.list);
	while (!kthread_should_stop()){
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
					// kfree(temp) not sure to put this in here
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

		msleep(1000);
		list_for_each(p, &(myTable.list)){
			ts = list_entry(p, struct processTable, list);
			ts->pCount = 0;
		}

	}
	return 0;
}

// takes parameter name of target. Goes through all the tasks using
// for_each_process and eliminates all processes that have the target
// name

void kill_process(char* target){

	for_each_process(task) {
		if (strcmp(task->comm, target) == 0) {
			printk("KILLED: %d\n", task->pid);
			send_sig(SIGKILL, task, 0);
		}
	}
}

static int __init thrash_detection(void){
	int data = 20;

	// We can instantiate multiple threads, but I think one should suffice?
	task = kthread_run(
		&my_kthread_function,
		(void*)data,
		"my_kthread");

	return 0;
}

static void __exit thrash_detection_exit(void){
	kthread_stop(task);
}

module_init(thrash_detection);
module_exit(thrash_detection);