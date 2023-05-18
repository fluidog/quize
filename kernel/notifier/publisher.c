#include <linux/kernel.h>
#include <linux/notifier.h>


static BLOCKING_NOTIFIER_HEAD(my_notify_list);

void do_notify(void)
{
    blocking_notifier_call_chain(&my_notify_list, 0, NULL);
}

int register_my_notifier(struct notifier_block *nb)
{
    return blocking_notifier_chain_register(&my_notify_list, nb);
}
EXPORT_SYMBOL_GPL(register_my_notifier);

int unregister_my_notifier(struct notifier_block *nb)
{
    return blocking_notifier_chain_unregister(&my_notify_list, nb);
}
EXPORT_SYMBOL_GPL(unregister_my_notifier);
