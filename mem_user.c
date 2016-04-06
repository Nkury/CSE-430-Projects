// mem_user.c test program

#include <linux/unistd.h>
#include <linux/stdio.h>
#include <linux/stdlib.h>

#define __NR_my_syscall 359

int main (int argc, char *argv[])
{
	int pid = 0;
	uint32_t virtAddr = 0;
	char *p;
	long address;
	
	if (argc != 3) {
		printf("Two arguments are expected in the form of <PID> <Virtual_Address>.");
		exit(1);
	}
	else {
		pid = atoi(argv[1]);
		virtAddr = strtoul(argv[2], &p, 16);	// Not working
		address = syscall(__NR_my_syscall, pid, virtAddr);
	}

	if (address == -1)
		printf("That page is not available.\n");
	else {
		printf("The address is %ld.\n", address);
		printf("The address is 0x%lx.\n", address);
	}

	return 0;
}

// End mem_user.c
