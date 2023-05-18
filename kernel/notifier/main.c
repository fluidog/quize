#include <linux/kernel.h>


void do_notify(void);

int init_subscriber(void);
void exit_subscriber(void);


int  init_main(void)
{
	init_subscriber();
	do_notify();
	do_notify();
	
	return 0;
}
void exit_main(void)
{
	exit_subscriber();
}
