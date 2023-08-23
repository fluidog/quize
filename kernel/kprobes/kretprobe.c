
#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/ktime.h>
#include <linux/types.h>

/* per-instance private data */
struct my_data
{
    ktime_t entry_stamp;
};

static int entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct my_data *data;

    // only 'cat' command
    if(strcmp(current->comm, "cat") != 0)
        return 0;

    if (!current->mm)
        return 1; /* Skip kernel threads */

    data = (struct my_data *)ri->data;
    data->entry_stamp = ktime_get();
    return 0;
}

/*
 * Return-probe handler: Log the return value and duration. Duration may turn
 * out to be zero consistently, depending upon the granularity of time
 * accounting on the platform.
 */
static int ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    int retval = regs_return_value(regs);
    struct my_data *data = (struct my_data *)ri->data;
    s64 delta;
    ktime_t now;

    if(strcmp(current->comm, "cat") != 0)
        return 0;

    now = ktime_get();
    delta = ktime_to_ns(ktime_sub(now, data->entry_stamp));

    pr_info("func: %s returned %d and took %lld ns to execute\n",
            "test", retval, (long long)delta);
    return 0;
}

static struct kretprobe my_kretprobe = {
    .handler = ret_handler,
    .entry_handler = entry_handler,
    .data_size = sizeof(struct my_data),
    /* Probe up to 20 instances concurrently. */
    .maxactive = 20,
    .kp.symbol_name = "__x64_sys_openat",
};

int init_test_kretprobe(void)
{
    int ret;
    ret = register_kretprobe(&my_kretprobe);
    if (ret < 0)
    {
        pr_info("register_kretprobe failed, returned %d\n",ret);
        return ret;
    }

    pr_info("Planted kernel return probe at %px", my_kretprobe.kp.addr);

    return 0;
}

void exit_test_kretprobe(void)
{
    unregister_kretprobe(&my_kretprobe);
}
