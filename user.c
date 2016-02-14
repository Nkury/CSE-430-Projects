#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __NR_my_syscall 359

struct processInfo{
	char id[16];
	char time[16];
	char comm[16];
	char name[64];
} proc;

// forward declaration
void parseProcess(char[]);

int main()
{
	char buffer[1024]; // initializes it to a value that is not null (?)
	int index = 0;
	printf("  PID\tTTY\t\t\tTIME\tCMD"); // header of the printed processes
	// check if index = 0 so it can enter the loop in the first place
	while (buffer != NULL || index == 0){
		int ret = syscall(__NR_my_syscall, index, buffer);
		if (buffer != NULL || index == 0){
			parseProcess(buffer);
			printf("  %s\t%s\t\t\t%s\t%s", proc.id, proc.name, proc.time, proc.comm);
			index++;
		}
	}
	return 0;
}

// parse the space delimited buffer
void parseProcess(char buff[]){
	char input[1024]; // to store input

	// what to store
	int selection = 0;
	int lastIndex = 0;
	int i, j;
	for (i = 0; i < 1024; i++){
		if (buff[i] != ' '){
			input[i - lastIndex] = buff[i];
			selection++;
		}
		else{
			lastIndex = i+1; // +1 to take into account space
			switch (selection){
				case 0: strcpy(proc.id, input);
					break;
				case 1: strcpy(proc.name, input);
					break;
				case 2: strcpy(proc.time, input);
					break;
				case 3: strcpy(proc.comm, input);
					selection = 0;
					break;
			}

			for (j = 0; j < 1024; j++)
				input[j] = '\0';
		}
	}
}
