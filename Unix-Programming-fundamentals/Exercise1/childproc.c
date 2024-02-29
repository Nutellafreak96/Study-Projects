#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int max_processes = sysconf(_SC_CHILD_MAX);
	printf("Max number of child processes: %d\n",max_processes);
	return 0;

}
