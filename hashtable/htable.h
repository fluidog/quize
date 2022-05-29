

struct htable {
	struct hlist_head **head;	/* hash table */
	u32 size;			/* number of slots in hash table */
	u32 nel;			/* number of elements in hash table */
	/* hash function */
	u32 (*hash_value)(struct htable *h, const void *key);
	/* key comparison function */
	int (*keycmp)(struct htable *h, const void *key1, const void *key2);
	/* operation */
	struct {
		int (*insert)(struct htable *h, void *key, void *data);
		void (*remove)(struct htable *h, void *key);
		void *(*search)(struct htable *h, const void *key);
		void (*destroy)(struct htable *h);
		int (*map)(struct hashtab *h,
			int (*apply)(void *k, void *d, void *args),
			void *args);
	};
};

struct htable *htable_create(u32 (*hash_value)(struct htable *h, const void *key),
					int (*keycmp)(struct htable *h, const void *key1, const void *key2),
					struct htable_callback *callback,  u32 size);

