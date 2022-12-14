/**
 * @file main.c
 * @author liuqi (liuqi1@kylinos.cn)
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define DEBUG
#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/timer.h>

static void timer_handler(struct timer_list *t);
DEFINE_TIMER(timer, timer_handler);
static unsigned long timer_interval = 2 * 1000; // 2ms

static void timer_handler(struct timer_list *t)
{
	pr_debug("timer handler start.\n");
	/*Restarting the periodic timer...*/
    mod_timer(t, jiffies + msecs_to_jiffies(timer_interval));

	// do something...

	pr_debug("timer handler end.\n");
}


int __init init_main(void)
{
	int error = 0;
	pr_info("Hello module\n");

	timer.expires = jiffies + msecs_to_jiffies(timer_interval);
	add_timer(&timer);

	pr_info("Periodic timer init success! period: %ld ms\n", timer_interval);
	return error;
}


void __exit exit_main(void)
{
	del_timer(&timer);

	pr_info("Goodbye module!\n");
}


module_init(init_main);
module_exit(exit_main);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liuqi");
MODULE_VERSION("v0.1");
MODULE_DESCRIPTION("timer example");
