#ifndef _DTN_LIB
#define _DTN_LIB

#define DTN_DAT "dat/dtn.dat"

typedef struct btree_key {
    char* s;
} BT_key;

typedef struct btree_val{
    char* s;
} BT_val;

typedef struct btree_item{
    BT_key k; BT_val v;
} BT_item;

typedef struct btree_dict* BT_dict;

extern BT_dict _delg_init_dtn(char* fname);
extern BT_dict init_dtn();

extern int dtn_insert(BT_dict dict, BT_key key, BT_val val);
extern BT_val* dtn_search(BT_dict dict, BT_key key);
extern BT_item* dtn_delete(BT_dict dict, BT_key key);

extern BT_key* dtn_suggest(BT_dict dict, BT_key key, int* size, int max_size);

extern void dtn_free(BT_dict dict);

#endif /* _DTN_LIB */