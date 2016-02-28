#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/sched.h>		/* Needed for 'for_each_process' */
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/signal.h>

struct task_struct *task;

int my_kthread_function(void* data){
	while (!kthread_should_stop()){
		// most of the time kthread sleeps
		msleep(1000);
		read_lock(&tasklist_lock); // lock the task_struct
		struct list_head *p;
		struct task_struct ts;
		for_each_process(task){
			list_for_each(p, &(task->children)){
				ts = *list_entry(p, struct task_struct, sibling);
				//** do something with data structure Grant is creating
			}
		}
		read_unlock(&tasklist_lock);
		//** check Grant's data structure for depth greater than 3
		//** call Jackson's kill command
		//** stop the kthread somehow
	}
	return 0;
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
