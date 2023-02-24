
#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>

#include <linux/slab.h>

struct data_node
{
	struct list_head list;
	/* data */
	int value;
};

static LIST_HEAD(data_head);

int add_data(int value)
{
	struct data_node *node = kmalloc(sizeof(struct data_node), GFP_KERNEL);
	if (IS_ERR_OR_NULL(node))
		return PTR_ERR(node);

	node->value = value;
	list_add(&node->list, &data_head);

	return 0;
}

void del_data(int value)
{
	struct data_node *node, *tmp;

	list_for_each_entry_safe(node, tmp, &data_head, list){
		if (node->value == value)
		{
			list_del(&node->list);
			kfree(node);
		}
	}
}

void clear_data(void)
{
	struct data_node *node, *tmp;

	list_for_each_entry_safe(node, tmp, &data_head, list){
		list_del(&node->list);
		kfree(node);
	}
}

void print_data(void)
{
	struct data_node *node;

	list_for_each_entry(node, &data_head, list){
		pr_info("value: %d", node->value);
	}
}


int __init init_main(void)
{
	int error = 0;
	add_data(1);
	add_data(25);
	add_data(38);

	print_data();
	
	return error;
}


void __exit exit_main(void)
{
	del_data(25);
	print_data();

	clear_data();
	print_data();
}
