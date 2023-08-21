// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kthread.h>

/*
 * Any file that uses trace points, must include the header.
 * But only one file, must include the header by defining
 * CREATE_TRACE_POINTS first.  This will make the C code that
 * creates the handles for the trace points.
 */
#define CREATE_TRACE_POINTS
#include "trace-events-sample.h"

static int simple_thread(void *arg)
{
	int cnt = 0;

	while (!kthread_should_stop()){

		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(HZ);

		trace_testevent("hello", cnt++);
	}

	return 0;
}

static struct task_struct *simple_tsk;

int init_main(void)
{
	simple_tsk = kthread_run(simple_thread, NULL, "event-sample");
	if (IS_ERR(simple_tsk))
		return -1;
	return 0;
}

void exit_main(void)
{
	kthread_stop(simple_tsk);
}