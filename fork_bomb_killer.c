#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */

static int __init fork_bomb_killer(void){
	//printk(KERN_INFO "Nizar Kury\n");
	return 0;
}

static void __exit fork_bomb_killer_exit(void){
	//printk(KERN_INFO "Exiting...\n");
}

module_init(fork_bomb_killer);
module_exit(fork_bomb_killer_exit);