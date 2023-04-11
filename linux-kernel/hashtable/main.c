#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/hashtable.h>
#include <linux/slab.h>

#define SLOTS 8 // 哈希表长度为 1 << 8
static DEFINE_HASHTABLE(my_head, SLOTS);

struct my_data
{
	struct hlist_node hlist_node;
	/* data */
	int key;
	int value;
};

// static inline int fast_hash(const char *data, unsigned int len)
// {
// 	int hashval = 0;
// 	for(; len > 0; len--){
// 		// hashval = ((hashval << 4) | (hashval >> (8 * sizeof(u32) - 4))) ^ (*key++);
// 		hashval += *data++;
// 	}
// 	return hashval;
// };

int add_data(int key)
{
	struct my_data *data = kmalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->key = key;
	hash_add(my_head, &data->hlist_node, key);

	return 0;
}

void del_data(int key)
{
	struct my_data *data;
	struct hlist_node *tmp;
	hash_for_each_possible_safe(my_head, data, tmp, hlist_node, key)
	{
		if (data->key == key)
		{
			hash_del(&data->hlist_node);
			kfree(data);
		}
	}
}

struct my_data *find_data(int key)
{
	struct my_data *data;
	hash_for_each_possible(my_head, data, hlist_node, key)
	{
		if (data->key == key)
		{
			return data;
		}
	}
	return NULL;
}

void clear_data(void)
{
	int bkt;
	struct my_data *data;
	struct hlist_node *tmp;
	hash_for_each_safe(my_head, bkt, tmp, data, hlist_node)
	{
		hash_del(&data->hlist_node);
		kfree(data);
	}
}

void print_data(void)
{
	int bkt;
	struct my_data *data;
	hash_for_each(my_head, bkt, data, hlist_node)
	{
		pr_info("bkt:%d, key: %d", bkt, data->key);
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
