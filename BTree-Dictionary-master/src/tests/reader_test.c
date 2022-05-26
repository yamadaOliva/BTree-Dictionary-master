#include "../dict/dict.h"
#include "../dict/suggest.h"
#include "../dict/tsv_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int err_num = 0; //So loi xuat hien

    BT_dict dtn = init_dtn();
    BT_sndx sndx = init_sndx();

    dtn_read_tsv(dtn, sndx, "dict.tsv");
}