
#include "obj.h"

int hello(int x, int y)
{
	return x + y;
}

int main()
{
	int x,y;

	x = obj1_f1(10);

	obj2_a = 20;
	y= obj2_f1(10);
	
	x = hello(x,y);
	return 0;
}