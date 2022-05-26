#ifndef _SUGGEST_H
#define _SUGGEST_H

#include "dict.h"

#define SDX_DAT "dat/sdxmap.dat"

typedef struct btree_soundex* BT_sndx;

extern BT_sndx init_sndx();

extern int sndx_insert(BT_sndx soundex_tree, BT_key key);
extern BT_item* sndx_delete(BT_sndx soundex_tree, BT_key key);
extern BT_key* sndx_suggest(BT_sndx soundex_tree, BT_key key, int* size, int max_size);

extern void sndx_free(BT_sndx soundex_tree);

#endif /* _SUGGEST_H */