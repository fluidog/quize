#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

static LIST_HEAD(my_head);
struct my_data
{
	struct list_head list_node;
	/* data */
	int value;
};

int add_data(int value)
{
	struct my_data *data = kmalloc(sizeof(*data), GFP_KERNEL);
	if (IS_ERR_OR_NULL(data))
        return PTR_ERR(data) ?: -1;

	data->value = value;

	list_add(&data->list_node, &my_head);

	return 0;
}

void del_data(int value)
{
	struct my_data *data, *tmp;

	list_for_each_entry_safe(data, tmp, &my_head, list_node)
	{
		if (data->value == value)
		{
			list_del(&data->list_node);
			kfree(data);
		}
	}
}

void clear_data(void)
{
	struct my_data *data, *tmp;

	list_for_each_entry_safe(data, tmp, &my_head, list_node)
	{
		list_del(&data->list_node);
		kfree(data);
	}
}

void print_data(void)
{
	struct my_data *data;

	list_for_each_entry(data, &my_head, list_node)
	{
		pr_info("value: %d", data->value);
	}
}

int __init init_main(void)
{
	pr_info("add 1, 25, 38\n");
	add_data(1);
	add_data(25);
	add_data(38);
	print_data();

	pr_info("del 25\n");
	del_data(25);
	print_data();

	pr_info("clear all data\n");
	clear_data();
	print_data();

	return 0;
}
