#include <linux/kernel.h>
#include <linux/notifier.h>

int register_my_notifier(struct notifier_block *);
int unregister_my_notifier(struct notifier_block *);

int my_nb_call(struct notifier_block *nb, unsigned long action, void *data)
{
    pr_info("my_notifier_call: action=%lu", action);
    return 0;
}

static struct notifier_block my_nb = {
    .notifier_call = my_nb_call
};


int init_subscriber(void)
{
    return register_my_notifier(&my_nb);
}

void exit_subscriber(void)
{
    unregister_my_notifier(&my_nb);
}



