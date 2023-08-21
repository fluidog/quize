#define pr_fmt(fmt) "%s: " fmt, __func__
#include <linux/kernel.h>

int init_test_kprobe(void);
void exit_test_kprobe(void);

int init_test_kretprobe(void);
int exit_test_kretprobe(void);


int  init_main(void)
{
	init_test_kprobe();
	init_test_kretprobe();
	return 0;
}

void  exit_main(void)
{
	exit_test_kprobe();
	exit_test_kretprobe();

}