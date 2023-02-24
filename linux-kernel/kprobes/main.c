#define pr_fmt(fmt) "%s: " fmt, __func__
#include <linux/kernel.h>

int test_kprobe(void);
int test_kretprobe(void);

int test(int a, int b)
{
	pr_info("a:%d, b:%d\n", a, b);
	return a+b;
}

int  init_main(void)
{
	test_kprobe();
	test_kretprobe();
	return 0;
}
void  exit_main(void)
{
}