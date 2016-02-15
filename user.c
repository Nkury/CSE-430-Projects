#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __NR_my_syscall 359


// forward declaration
void parseProcess(char[]);

int main()
{

	char buffer[800]; // initialize buffer
	int numTasks; // store number of processes in system
	int index = 0; // for looping system calls
	numTasks = syscall(__NR_my_syscall, index, buffer); // first call to get size of tasks
	printf("  PID\tTTY\t\t\tTIME\tCMD\n"); // header of the printed processes
	// check if index = 0 so it can enter the loop in the first place
	while (index < numTasks){
		memset(buffer, 0, 800); // set buffer to null (reset)
		int ret = syscall(__NR_my_syscall, index, buffer); // call syscall for every task
		printf("  %s\n", buffer); // print the buffer
		index++; //increment buffer
	}

	return 0;
}


