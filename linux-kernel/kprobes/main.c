/**
 * @file main.c
 * @author liuqi (liuqi1@kylinos.cn)
 * @brief 
 * @version 0.1
 * @date 2022-12-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/kprobes.h>


static struct kprobe kp = {
	.symbol_name = "test",
};

int test(int a, int b)
{
	pr_info("a:%d, b:%d\n", a, b);
	return 0;
}

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	pr_info("p->addr = 0x%p, ip = %lx, flags = 0x%lx, a1 = %lx, a2 = %lx",
		p->addr, regs->ip, regs->flags, 
		regs_get_kernel_argument(regs,0), regs_get_kernel_argument(regs,1));
	return 0;
}

static void handler_post(struct kprobe *p, struct pt_regs *regs,
				unsigned long flags)
{
	pr_info("p->addr = 0x%p, ip= %lx, flags = 0x%lx, a1 = %lx, a2 = %lx",
		p->addr, regs->ip, regs->flags, 
		regs_get_kernel_argument(regs,0), regs_get_kernel_argument(regs,1));
}


static int __init kprobe_init(void)
{
	int ret;

	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;

	ret = register_kprobe(&kp);

	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	pr_info("Planted kprobe at %p", kp.addr);

	test(5, 46);

	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init);
module_exit(kprobe_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("liuqi");
MODULE_VERSION("v0.1");
MODULE_DESCRIPTION("kprobes example");
