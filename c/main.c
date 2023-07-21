#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main(int argc, char* argv[])
{
	printf("name: %s\n", argv[0]);

	for(int i = 1; i < argc; i++)
		printf("arg[%d]: %s\n", i, argv[i]);

	return 0;
}

