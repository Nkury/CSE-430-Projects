#include <linux/syscalls.h>
#include <linux/kernel.h> /* Needed for KERN_INFO*/

asmlinkage long sys_my_syscall(int i){
	printk("This is a new system call...");
	return ((long)i);
}