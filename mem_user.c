// mem_user.c test program

#include <linux/unistd.h>
#include <linux/stdio.h>

#define __NR_my_syscall 359

int main ()
{
	int pid;
	long virtAddr;
	
	printf("PID: ");
	scanf("%d", &pid);
	printf("Virtual Address: ");
	scanf("%d", &virtAddr);

	long address = syscall(__NR_my_syscall, pid, vAddr);

	if (address == -1)
		printf("That page is not available.\n");
	else
		printf("The address is %ld.", address);

	return 0;
}

// End mem_user.c
