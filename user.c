#include <linux/unistd.h>
#define __NR_my_syscall 359

int main()
{
	int a, b = 0;
	int* user_pids;
	int* user_times;
	char* user_comm[];
	char* user_name[];
	
	int ret = syscall(__NR_my_syscall, user_pids, user_times, user_comm, user_name);
	
	int i = 0;
	
	while (user_pids[i] != '\0' & user_times[i] != '\0' & user_comm[i] != '\0' & user_name[i] != '\0')
	{
			for(i = 0; i < 10000; i++)
				printf("%d, %d, %s, %s\n", user_pids[i], user_times[i], user_comm[i], user_name[i]);
	}
	
	return 0;
}
