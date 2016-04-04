#include <linux/syscalls.h>
#include <linux/kernel.h> /* Needed for KERN_INFO*/
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/tty.h>
#include <linux/jiffies.h>
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

asmlinkage long sys_my_syscall(int pid, long virtAddr){

	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	spinlock_t *ptl;
	pte_t *ptep, pte;

	struct task_struct *task;	// instantiate an instance of task from task_struct that will be traversed in the following loop

	// we loop through each of the tasks
	for_each_process(task){
		if (pid == task->pid){
			pgd = pgd_offset(task->mm, virtAddr);

			// checks if there is a valid entry in the page global directory
			if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd))){
				return -1;
			}

			pud = pud_offset(pgd, virtAddr);

			// checks if there is a valid entry in the page upper directory
			if (pud_none(*pud) || unlikely(pud_bad(*pud))){
				return -1;
			}

			pmd = pmd_offset(pud, virtAddr);

			// checks if there is a valid entry in the page middle directory
			if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd))){
				return -1;
			}

			ptep = pte_offset_map_lock(task->mm, pmd, virtAddr, &ptl);

			if (pte_none(pte))
				return -4;

			pte = *ptep;

			pte_unmap_unlock(ptep, ptl);
			// checks if there is a valid page table entry
			if (pte_present(pte)){

				// PHYSICAL MEMORY ADDRESS
				// SHIFT LEFT 12 bits and then
				// add last 12 bits of virtual address
				// to the memory address of pte
				return pte_pfn(pte);
			}
			else{
				return pte_pfn(pte);
			}

		}
	}

	return -1; // returns -1 if copied successfully
}

