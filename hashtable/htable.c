
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/list.h>

#include "htable.h"


struct htable_node {
	void *key;
	void *data;
	struct hlist_node list;
};

static u32 general_hash_value(struct htable *h, const void *key)
{
	const char *p, *keyp;
	unsigned int size;
	unsigned int val;

	val = 0; 
	keyp = key;
	size = strlen(keyp);
	for (p = keyp; (p - keyp) < size; p++)
		val = (val << 4 | (val >> (8*sizeof(unsigned int)-4))) ^ (*p);
	return val & (h->size - 1);
}

static int general_keycmp(struct htable *h, const void *key1, const void *key2)
{
	return strcmp((char *)key1, (char *)key2);
}

static int general_insert(struct htable *h, void *key, void *data)
{
	u32 index;
	struct htable_node node;
	int result;
	if(!h)
		return -EINVAL;

	index = h->hash_value(h, key);

	result = EEXIST;
	hlist_for_each_entry(node, h->htable[index], list){
		if(!h->keycmp(h, key, node->key))	
			goto end;
	}

	result = -ENOMEM;
	node = kzalloc(sizeof(*node), GFP_KERNEL);
	if(!node)
		goto end;

	node->key = key;
	node->data = data;
	hlist_add_head(&node->list, node->head[index]);

	return 0;
}

static void general_remove(struct htable *h, void *key)
{
	u32 index;
	struct htable_node *node;
	if(!h)
		return ;

	index = h->hash_value(h, key);
	hlist_for_each_entry(node, h->htable[index], list){
		if(!h->keycmp(h, key, node->key)){
			hlist_del(&node->list);
			kfree(node);
			break;
		}
	}
}

static void *general_search(struct htable *h, const void *key)
{
	u32 index;
	struct htable_node *node;
	void *result = NULL;
	if (!h)
		return NULL;

	index = h->hash_value(h, key);
	hlist_for_each_entry(node, h->htable[index], list){
		if(!h->keycmp(h, key, node->key)){
			result = node->data;
			break;
		}
	}
	return result;
}

static void general_destroy(struct htable *h)
{
	u32 index;
	struct htable_node *node;
	struct hlist_node *n;
	void *key;
	if(!h)
		return ;

	for(index=0; index < h->size; index++){
		hlist_for_each_entry_safe(node, n, h->htable[index], list){
			key = node->key;
			if(h->callback && h->callback->presrch)
				h->callback->prerm(h, key);

			hlist_del(&node->list);
			kfree(node);

			if(h->callback && h->callback->postsrch)
				result = h->callback->postrm(h, key);
		}
	}
}

static int general_map(struct hashtab *h,
		int (*apply)(void *k, void *d, void *args),
		void *args)
{
	u32 i;
	int ret;
	struct htable_node *node;

	if (!h)
		return 0;

	for (i = 0; i < h->size; i++) {
		cur = h->htable[i];
		while (cur) {
			ret = apply(cur->key, cur->datum, args);
			if (ret)
				return ret;
			cur = cur->next;
		}
	}
	return 0;
}

struct htable *htable_create(u32 (*hash_value)(struct htable *h, const void *key),
					int (*keycmp)(struct htable *h, const void *key1, const void *key2),
					  u32 size)
{
	struct htable *h;
	u32 i;

	h = kmalloc(sizeof(*h), GFP_KERNEL);
	if (h == NULL)
		return h;

	h->nel = 0;
	h->hash_value = general_hash_value;
	h->keycmp = general_keycmp;
	h->insert = general_insert;
	h->remove = general_remove;
	h->search = general_search;
	h->destroy = general_destroy;
	h->dump = general_dump;
	h->map = general_map;

	if(hash_value)
		h->hash_value = hash_value;
	if(keycmp)
		h->keycmp = keycmp;

	h->size = size;
	h->head = kmalloc(sizeof(*(h->head)) * size, GFP_KERNEL);
	if (h->head == NULL) {
		kfree(h);
		return NULL;
	}

	for (i = 0; i < size; i++)
		INIT_HLIST_HEAD(h->head[i]);	

	return h;
}
