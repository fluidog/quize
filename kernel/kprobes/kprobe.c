
#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/kprobes.h>

#ifdef CONFIG_X86_64
static inline unsigned long regs_get_kernel_argument(struct pt_regs *regs,
						     unsigned int n)
{
	static const unsigned int argument_offs[] = {
#ifdef __i386__
		offsetof(struct pt_regs, ax),
		offsetof(struct pt_regs, cx),
		offsetof(struct pt_regs, dx),
#define NR_REG_ARGUMENTS 3
#else
		offsetof(struct pt_regs, di),
		offsetof(struct pt_regs, si),
		offsetof(struct pt_regs, dx),
		offsetof(struct pt_regs, cx),
		offsetof(struct pt_regs, r8),
		offsetof(struct pt_regs, r9),
#define NR_REG_ARGUMENTS 6
#endif
	};

	if (n >= NR_REG_ARGUMENTS) {
		n -= NR_REG_ARGUMENTS - 1;
		return regs_get_kernel_stack_nth(regs, n);
	} else
		return regs_get_register(regs, argument_offs[n]);
}
#endif

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
