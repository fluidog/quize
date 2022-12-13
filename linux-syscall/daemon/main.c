
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int i;
	pid_t pid = fork();
	if(pid < 0){
		perror("fork");
		exit(-1);
	}

	// Parent goes bye
	if(pid != 0)
		exit(0);

	// Child continues
	// Become session leader
	if(setsid() < 0){
		perror("setsid");
		exit(-1);
	}

	// Close all descriptors, you will can't use printf() or perror() anymore.
	for(i = 0; i < 64; i++)
		close(i);

	// Change working directory
	chdir("/");

	// Clear file mode creation mask
	umask(0);

	// Do your work here
	// ...
	while(1){

	}

	return 0;
}
