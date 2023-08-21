
#define CREATE_TRACE_POINTS
#include "trace-point-sample.h"

static void cb(void *data, int num, struct task_struct *p)
{
    pr_info("num: %d, comm: %s\n", num, p->comm);
}

int init_main(void)
{
    trace_testevent(15, current);

    if( register_trace_testevent(cb, NULL) < 0 )
        return -1;

    trace_testevent(30, current);
    
    return 0;
}
