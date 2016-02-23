#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/sched.h>	/* Needed for 'for_each_process' */

static int __init fork_bomb_killer(void){
	printk(KERN_INFO "Fork Bomb Squad Deployed.\n");
	return 0;
}

static void __exit fork_bomb_killer_exit(void){
	printk(KERN_INFO "Cleaning Up.\n");
}

module_init(fork_bomb_killer);
module_exit(fork_bomb_killer_exit);
