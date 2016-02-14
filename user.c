//#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define __NR_my_syscall 359

struct processInfo{
	char id[16];
	char time[16];
	char comm[16];
	char name[64];
};

int main()
{
	char buffer[1024] = ""; // initializes it to a value that is not null (?)
	struct processInfo proc;
	int index = 0;
	printf("  PID\tTTY\t\t\tTIME\tCMD"); // header of the printed processes
	while (buffer != NULL){
		int ret = syscall(__NR_my_syscall, index, buffer);
		if (buffer != NULL){
			proc = parseProcess(buffer);
			printf("  %s\t%s\t\t\t%s\t%s", proc.id, proc.name, proc.time, proc.comm);
			index++;
		}
	}
	return 0;
}

// parse the space delimited buffer
struct processInfo parseProcess(char buff[]){
	char input[1024]; // to store input
	struct processInfo proc;// = (struct processInfo)malloc(sizeof(struct processInfo));
	// what to store
	int selection = 0;
	int lastIndex = 0;
	int i;
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
					break;
			}
			input = "";
		}
	}

	return proc;
}
