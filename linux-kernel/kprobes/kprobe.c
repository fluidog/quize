
#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>

#include <linux/kprobes.h>

int test(int a, int b);

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	pr_info("p->addr = %px, ip = %px, flags = 0x%lx, arg = %ld,%ld...",
			p->addr, (void *)regs->ip, regs->flags,
			regs_get_kernel_argument(regs, 0), regs_get_kernel_argument(regs, 1));
	return 0;
}

static void handler_post(struct kprobe *p, struct pt_regs *regs,
						 unsigned long flags)
{
	pr_info("p->addr = 0x%px, ip= %px, flags = 0x%lx, arg = %ld,%ld...",
			p->addr, (void *)regs->ip, regs->flags,
			regs_get_kernel_argument(regs, 0), regs_get_kernel_argument(regs, 1));
}

static struct kprobe kp = {
	.symbol_name = "test",
	.offset = 0,
	.pre_handler = handler_pre,
	.post_handler = handler_post,
};

int test_kprobe(void)
{
	int ret;

	ret = register_kprobe(&kp);

	if (ret < 0)
	{
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	pr_info("Planted kernel probe at %px", kp.addr);

	test(5, 46);

	unregister_kprobe(&kp);

	return 0;
}