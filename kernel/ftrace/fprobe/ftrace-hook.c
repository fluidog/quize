#include <linux/ftrace.h>

/* 定义 ftrace hook 函数 */
static void my_ftrace_handler(unsigned long ip, unsigned long parent_ip, struct ftrace_ops *ops, struct ftrace_regs *regs)
{
    // only 'cat' command
    if(strcmp(current->comm, "cat") != 0)
        return;
    trace_printk("pid=%d comm=%s ip=%pBn\n", 
        current->pid, current->comm, (void *)ip);
}

/* 定义 ftrace 操作结构体 */
static struct ftrace_ops my_ftrace_ops = {
    .func = my_ftrace_handler,
    .flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_RECURSION,
};

int init_ftrace_hook(void)
{
    int ret=0;

    ret = ftrace_set_filter(&my_ftrace_ops, "__x64_sys_openat", strlen("__x64_sys_openat"), 0);
    if (ret) {
        printk(KERN_ERR "Failed to set ftrace filter\n");
        return ret;
    }

    /* 注册 ftrace 操作 */
    ret = register_ftrace_function(&my_ftrace_ops);
    if (ret) {
        printk(KERN_ERR "Failed to register ftrace function\n");
        return ret;
    }

    return 0;
}

void exit_ftrace_hook(void)
{
    /* 取消注册 ftrace 操作 */
    unregister_ftrace_function(&my_ftrace_ops);
}