#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H   

#include <stdlib.h>

typedef struct process_info {
	int pid; // process id
	int time_in_seconds; // for CPU time
	char comm[16]; // TASK_COMMON_LEN = 16 according to LXR
	char name[64]; // for TTY
}process_info;



typedef struct process_tree {
	process_info info;
	process_tree* left;
	process_tree* right;
}*head;

#endif