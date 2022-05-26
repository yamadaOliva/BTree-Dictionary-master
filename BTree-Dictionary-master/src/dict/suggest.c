#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "suggest.h"

#include "../btree-5.0.1/inc/btree.h"
#include "../soundex/soundex.h"

#define MAX_SIZE 2000
#define MAX_LENGTH 30

struct btree_soundex {
    BTA bt;
};

BT_sndx init_sndx() {
    soundex_h_init();
    return (BT_sndx)_delg_init_dtn(SDX_DAT);
}

char* non_alpha_soundex(const char* s) {
    int i = 0;
    int j = 0;

    char* n = strdup(s);
    while (i < (strlen(n) + 1))
    {
        if (isalpha(n[i]))
        {
            n[j] = n[i];
            ++j;
        }
        ++i;
    }
    n[j] = '\0';
    char* out = strdup(soundex(n));
    free(n);
    return out;
}

int sndx_insert(BT_sndx sndx, BT_key k) {
    BT_key sndx_k; sndx_k.s = non_alpha_soundex(k.s);
    BT_val v;
    BT_val* a;
    if(a = dtn_search((BT_dict)sndx, sndx_k)) {
        BT_item* item = dtn_delete((BT_dict)sndx, sndx_k);
        free(item->k.s); free(item->v.s); free(item);
        v.s = malloc(strlen(k.s) + 1 + strlen(a->s) + 1);
        v.s[0] = 0;
        strcat(v.s, k.s); strcat(v.s, " "); strcat(v.s, a->s);
        free(a->s);
    } else v.s = k.s;
    return dtn_insert((BT_dict)sndx, sndx_k, v);
}

BT_item* sndx_delete(BT_sndx sndx, BT_key k) {
    BT_key sndx_k; sndx_k.s = non_alpha_soundex(k.s);
    BT_item* item = dtn_delete((BTA*)sndx, sndx_k);
    free(sndx_k.s);
    
    if(item == NULL) return item;
    char * key = strtok(item->v.s, " ");

    int i = 0;
    while( key != NULL) {
        if(strcmp(key, k.s) != 0) {
            BT_key a; a.s = strdup(key);
            sndx_insert(sndx, a);
        }
        key = strtok(NULL, " ");
    }

}

BT_key* sndx_suggest(BT_sndx sndx, BT_key k, int* sze, int max) {
    char s[50];
    BT_key* keys = malloc(sizeof(BT_key) * max);

    BT_key sdx_k; sdx_k.s = non_alpha_soundex(k.s);
    BT_val* val = dtn_search((BT_dict)sndx, sdx_k);
    free(sdx_k.s);
    if(val == NULL) return keys;
    char * key = strtok(val->s, " ");
    *sze = 0;
    while( key != NULL && *sze < max) {
        keys[*sze].s = malloc(sizeof(key));
        strcpy(keys[(*sze)++].s, key);
        key = strtok(NULL, " ");
    }
    free(val->s); free(val);
    return keys;
}

void sndx_free(BT_sndx sndx) {
    dtn_free((BT_dict)sndx);
    return;
}