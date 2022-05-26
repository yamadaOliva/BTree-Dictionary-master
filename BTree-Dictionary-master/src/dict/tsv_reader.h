#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dict.h"
#include "suggest.h"

int dtn_read_tsv(BT_dict dtn, BT_sndx sndx, const char* fname) {
    char name[20];
    char ext[10];
    sscanf(fname, "%[^.].%s", name, ext);
    if(strcmp(ext, "tsv") != 0) {
        return 1;
    }
    FILE* is = fopen(fname, "r");
    char k[100];
    char v[5000];
    while(fscanf(is, "%[^\t]", k) != EOF) {
        fscanf(is, "\t%[^\n]\n", v);
        BT_key key; key.s = k;
        BT_val val; val.s = v;
        dtn_insert(dtn, key, val);
        sndx_insert(sndx, key);
        //printf("%s\t%s\n", k, v);
    }
    return 0;
}