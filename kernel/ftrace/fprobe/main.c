
extern int init_ftrace_fprobe(void);
extern void exit_ftrace_fprobe(void);

extern int init_ftrace_hook(void);
extern void exit_ftrace_hook(void);


int init_main(void)
{
    // return init_ftrace_fprobe();

    return init_ftrace_hook();
}

void exit_main(void)
{
    exit_ftrace_hook();

    // exit_ftrace_fprobe();
}