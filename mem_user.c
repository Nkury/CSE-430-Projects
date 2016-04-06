// mem_user.c test program

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define __NR_my_syscall 359

int main (int argc, char *argv[])
{
	int pid = 0;
	unsigned long virtAddr = 0;
	char *p;
	unsigned long address;
	
	if (argc != 3) {
		printf("Two arguments are expected in the form of <PID> <Virtual_Address>.");
		exit(1);
	}
	else {
		pid = atoi(argv[1]);
		virtAddr = strtoul(argv[2], &p, 16);
		address = syscall(__NR_my_syscall, pid, virtAddr);
	}

	printf("pid: %d, ", pid);
	printf("virtual address: 0x%lX, ", virtAddr);

	if (address == -1)
		printf("That page is not available.\n");
	else {
		//printf("The address is %lu.\n", address);	// testing
		printf("phys_addr: 0x%lX\n", address);
	}

	return 0;
}

// End mem_user.c
