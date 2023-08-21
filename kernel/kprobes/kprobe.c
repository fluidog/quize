
#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/kprobes.h>

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	// only 'cat' command
	if(strcmp(current->comm, "cat") != 0)
		return 0;

	pr_info("p->addr = %px, ip = %px, flags = 0x%lx, arg = %ld,%ld...",
			p->addr, (void *)regs->ip, regs->flags,
			regs_get_kernel_argument(regs, 0), regs_get_kernel_argument(regs, 1));
	return 0;
}

static struct kprobe kp = {
	.symbol_name = "__x64_sys_openat",
	.offset = 0,
	.pre_handler = handler_pre,
	// .post_handler = handler_post,
};

int init_test_kprobe(void)
{
	int ret;

	ret = register_kprobe(&kp);
	if (ret < 0)
	{
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}

	pr_info("Planted kernel probe at %px", kp.addr);

	return 0;
}

void exit_test_kprobe(void)
{
	unregister_kprobe(&kp);
}
