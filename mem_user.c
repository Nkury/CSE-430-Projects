// mem_user.c test program

#include <linux/unistd.h>
#include <linux/stdio.h>
#include <linux/stdlib.h>

#define __NR_my_syscall 359

int main (int argc, char *argv[])
{
	int pid;
	long virtAddr;
	
	if (argc != 3) {
		printf("Two arguments are expected in the form of <PID> <Virtual Address>.");
		exit(1);
	}
	else {
		pid = atoi(argv[1]);
		virtAddr = atol(argv[2]);
	}
	
	long address = syscall(__NR_my_syscall, pid, virtAddr);

	if (address == -1)
		printf("That page is not available.\n");
	else
		printf("The address is %ld.", address);

	return 0;
}

// End mem_user.c
