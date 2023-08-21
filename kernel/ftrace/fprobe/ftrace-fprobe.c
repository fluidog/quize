#include <linux/version.h>

// kernel version > 5.18
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,18,0)
  #warning "fprobe not support: kernel version < 5.18" 
  int init_ftrace_fprobe(void){ return 0; }
  void exit_ftrace_fprobe(void){ return ; }
#else

#include <linux/fprobe.h>

static void my_fprobe_entry_handler(struct fprobe *fp, unsigned long entry_ip, struct pt_regs *regs)
{
    // only 'cat' command
    if(strcmp(current->comm, "cat") != 0)
        return;

    trace_printk("enter: pid=%d comm=%s ip=%pBn\n", 
        current->pid, current->comm, (void *)entry_ip);
}

static void my_fprobe_exit_handler(struct fprobe *fp, unsigned long entry_ip, struct pt_regs *regs)
{
    // only 'cat' command
    if(strcmp(current->comm, "cat") != 0)
        return;

    trace_printk("exit: pid=%d comm=%s ip=%pBn\n", 
        current->pid, current->comm, (void *)entry_ip);
}

static struct fprobe fp = {
       .entry_handler  = my_fprobe_entry_handler,
       .exit_handler   = my_fprobe_exit_handler,
};

int init_ftrace_fprobe(void)
{
    int ret=0;
    char syms[] = {"__x64_sys_openat", "__x64_sys_open"};

    ret = register_fprobe_syms(&fp, syms, ARRAY_SIZE(syms));
    if (ret) {
        printk(KERN_ERR "Failed to register ftrace function\n");
        return ret;
    }

    return 0;
}

void exit_ftrace_fprobe(void)
{
    unregister_fprobe(&fp);
}

#endif