
#include <unistd.h>
#include <stdio.h>

int main()
{
	char buf[1024];
	int size;

	while(1)
	{
		size = read(0, buf, sizeof(buf));
		if(size <= 0)
			break;
		// printf("size: %d : %s\n",size, buf);
		write(2, buf, size);
		
		write(1, buf, size);
	}
	return size;
}