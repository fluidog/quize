#include <linux/kernel.h>
#include <linux/kthread.h>

static int simple_thread(void *arg)
{
	int cnt = 0;

	while (!kthread_should_stop()){

		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(HZ);

		pr_debug("hello %d\n", cnt++);
	}

	return 0;
}

static struct task_struct *simple_tsk;

int init_main(void)
{
	simple_tsk = kthread_run(simple_thread, NULL, "pr_debug_sample");
	if (IS_ERR(simple_tsk))
		return -1;
	return 0;
}

void exit_main(void)
{
	kthread_stop(simple_tsk);
}