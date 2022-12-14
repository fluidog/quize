
#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>

int __init init_main(void)
{
	int error = 0;
	pr_info("Hello module!\n");
	
	return error;
}


void __exit exit_main(void)
{

	pr_info("Goodbye module!\n");
}


module_init(init_main);
module_exit(exit_main);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liuqi");
MODULE_VERSION("v0.1");
MODULE_DESCRIPTION("module example");
