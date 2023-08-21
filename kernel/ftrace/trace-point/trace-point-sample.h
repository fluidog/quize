#undef TRACE_SYSTEM
#define TRACE_SYSTEM trace_point_sample

#if !defined(_TRACE_SUBSYS_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_SUBSYS_H

#include <linux/tracepoint.h>

DECLARE_TRACE(testevent,
	TP_PROTO(int num, struct task_struct *p),
	TP_ARGS(num, p));

#endif /* _TRACE_SUBSYS_H */


#undef TRACE_INCLUDE_PATH
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_PATH .
/*
 * TRACE_INCLUDE_FILE is not needed if the filename and TRACE_SYSTEM are equal
 */
#define TRACE_INCLUDE_FILE trace-point-sample

/* This part must be outside protection */
#include <trace/define_trace.h>