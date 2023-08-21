#undef TRACE_SYSTEM
#define TRACE_SYSTEM trace_events_sample

/*
 * Notice that this file is not protected like a normal header.
 * We also must allow for rereading of this file. The
 *
 *  || defined(TRACE_HEADER_MULTI_READ)
 *
 * serves this purpose.
 */
#if !defined(_TRACE_EVENT_SAMPLE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_EVENT_SAMPLE_H

/*
 * All trace headers should include tracepoint.h, until we finally
 * make it into a standard header.
 */
#include <linux/tracepoint.h>

TRACE_EVENT(testevent,

	TP_PROTO(const char *foo, int bar),

	TP_ARGS(foo, bar),

	TP_STRUCT__entry(
		__string(	foo,    foo		)
		__field(	int,	bar			)
	),

	TP_fast_assign(
		__assign_str(foo, foo);
		__entry->bar	= bar;
	),

	TP_printk("foo %s %d", __get_str(foo), __entry->bar)
);

#endif

/***** NOTICE! The #if protection ends here. *****/

/*
 * There are several ways I could have done this. If I left out the
 * TRACE_INCLUDE_PATH, then it would default to the kernel source
 * include/trace/events directory.
 *
 * I could specify a path from the define_trace.h file back to this
 * file.
 *
 * #define TRACE_INCLUDE_PATH ../../samples/trace_events
 *
 * But the safest and easiest way to simply make it use the directory
 * that the file is in is to add in the Makefile:
 *
 * CFLAGS_trace-events-sample.o := -I$(src)
 *
 * This will make sure the current path is part of the include
 * structure for our file so that define_trace.h can find it.
 *
 * I could have made only the top level directory the include:
 *
 * CFLAGS_trace-events-sample.o := -I$(PWD)
 *
 * And then let the path to this directory be the TRACE_INCLUDE_PATH:
 *
 * #define TRACE_INCLUDE_PATH samples/trace_events
 *
 * But then if something defines "samples" or "trace_events" as a macro
 * then we could risk that being converted too, and give us an unexpected
 * result.
 */
#undef TRACE_INCLUDE_PATH
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_PATH .
/*
 * TRACE_INCLUDE_FILE is not needed if the filename and TRACE_SYSTEM are equal
 */
#define TRACE_INCLUDE_FILE trace-events-sample

#include <trace/define_trace.h>
