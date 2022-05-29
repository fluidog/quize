#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main()
{
	int i = 0,x = 1000;
	char cwdbuf[100];
	char *p = getcwd(cwdbuf,100);
	for(i = 0; i< 1000; i++)
	{
		printf("hello world:%s\n",p);
		x--;
	}
	return 0;
}
