#include "dict.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../btree-5.0.1/inc/btree.h"

#define MAX_SIZE 2000
#define MAX_LENGTH 30

struct btree_dict {
    BTA bt;
};

BT_dict _delg_init_dtn(char* f) {
    BT_dict dtn;
    dtn = (BT_dict)btopn(f, 0, 0);
    if (!dtn) {
	    dtn = (BT_dict)btcrt(f, 0, 0);
    }
    return dtn;
}

BT_dict init_dtn() {
    return _delg_init_dtn(DTN_DAT);
}

int dtn_insert(BT_dict dtn, BT_key k, BT_val val) {
    int i = 0;
    while(k.s[i]) { 
        k.s[i] = tolower((unsigned char)(k.s[i]));
        i++;
    }
    return btins((BTA*)dtn, k.s, val.s, strlen(val.s) + 1);
}

BT_val* dtn_search(BT_dict dtn, BT_key k) {
    int size;
    BT_val* val = NULL;
    char s[5000];
    if(!btsel((BTA*)dtn, k.s, s, sizeof(s), &size)) {
        val = malloc(sizeof(BT_val));
        val->s = strdup(s);
    }
    return val;
}
BT_item* dtn_delete(BT_dict dtn, BT_key xk) {
    int size;
    BT_item* item = NULL;
    char s[5000];
    if(!btsel((BTA*)dtn, xk.s, s,sizeof(s), &size)) {
        item = malloc(sizeof(BT_item));
        item->k.s = strdup(xk.s); item->v.s = strdup(s);
        btdel((BTA*)dtn,xk.s);
    }
    return item;
}

BT_key* dtn_suggest(BT_dict dtn, BT_key k, int* sze, int max) {
    BT_key* keys = malloc(sizeof(BT_key) * max);
    *sze = 0; if(max == 0) return keys;
    
    BTint j;
    char s[5000];

    if(!bfndky((BTA*)dtn, k.s, &j))
        keys[(*sze)++].s = strdup(k.s);
    
    while((*sze) < max)
        if(!bnxtky((BTA*)dtn, s, &j))
            if(strncmp(k.s, s, strlen(k.s)) == 0) 
                keys[(*sze)++].s = strdup(s);
            else break;
        else break;

    return keys;
}

void dtn_free(BT_dict dtn) {
    btcls((BTA*)dtn);
    return;
}