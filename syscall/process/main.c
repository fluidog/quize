#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>


int main()
{
	printf("start\n");

	pid_t pid = fork();

	if (pid == 0)
	{
		printf("child\n");
	}
	else
	{
		printf("parent\n");
	}
	printf("end\n");
	return 0;
}
