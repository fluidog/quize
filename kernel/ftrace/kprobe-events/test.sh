tracefs=`grep debugfs /proc/mounts | awk '{ print $2; }'`/tracing

# ?: $arg1 != %ax ?
echo 'p:myprobe __x64_sys_openat %ax $arg1' > $tracefs/kprobe_events &&\
    echo 1 > $tracefs/events/kprobes/myprobe/enable

# echo 'r:myretprobe __x64_sys_openat $retval' >> /sys/kernel/debug/tracing/kprobe_events


# echo 0 > $tracefs/events/kprobes/myprobe/enable &&\
#  echo '-:myprobe' > $tracefs/kprobe_events