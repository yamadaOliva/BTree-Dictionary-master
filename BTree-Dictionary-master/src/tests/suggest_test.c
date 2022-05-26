#include "../dict/dict.h"
#include "../dict/suggest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BT_key make_key(char* s) {
    BT_key k; k.s = s; return k;
}
BT_val make_val(char* s) {
    BT_val v; v.s = s; return v;
}

int main() {
    int err_num = 0; //So loi xuat hien

    char keys[8][50] = {"-A", "Baa", "Baab", "Beev", 
                        "Ef", "EFss", "Gem", "Jem"}; 
    char vals[8][50] = {"a", "baa", "baab", "beev", 
                        "ef", "efss", "gem", "jem"}; 
    BT_dict dtn = init_dtn();
    BT_sndx sndx = init_sndx();

    dtn_insert(dtn, make_key(keys[0]), make_val(vals[0]));
    sndx_insert(sndx, make_key(keys[0]));
    dtn_insert(dtn, make_key(keys[1]), make_val(vals[1]));
    sndx_insert(sndx, make_key(keys[1]));
    dtn_insert(dtn, make_key(keys[2]), make_val(vals[2]));
    sndx_insert(sndx, make_key(keys[2]));
    dtn_insert(dtn, make_key(keys[3]), make_val(vals[3]));
    sndx_insert(sndx, make_key(keys[3]));
    dtn_insert(dtn, make_key(keys[4]), make_val(vals[4]));
    sndx_insert(sndx, make_key(keys[4]));
    dtn_insert(dtn, make_key(keys[5]), make_val(vals[5]));
    sndx_insert(sndx, make_key(keys[5]));
    dtn_insert(dtn, make_key(keys[6]), make_val(vals[6]));
    sndx_insert(sndx, make_key(keys[6]));
    dtn_insert(dtn, make_key(keys[7]), make_val(vals[7]));
    sndx_insert(sndx, make_key(keys[7]));
    
    int size = 0;
    BT_key* sggest = sndx_suggest(sndx, make_key(keys[2]), &size, 20);
    printf("Tim kiem tu giong %s: \n", keys[2]);
    for(int i = 0; i < size; ++i) {
        printf("%s,\n", sggest[i].s);
    }
}