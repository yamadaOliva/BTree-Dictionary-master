#include "../dict/dict.h"
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

    char keys[10][50] = {"A", "B", "C", "D", 
                        "E", "F", "G", "H", "M", "Sus"}; 
    char vals[8][50] = {"a", "b", "c", "d", 
                        "e", "f", "g", "h"}; 
    BT_dict dtn = init_dtn();
    dtn_insert(dtn, make_key(keys[0]), make_val(vals[0]));
    dtn_insert(dtn, make_key(keys[1]), make_val(vals[1]));
    dtn_insert(dtn, make_key(keys[2]), make_val(vals[2]));
    dtn_insert(dtn, make_key(keys[3]), make_val(vals[3]));
    dtn_insert(dtn, make_key(keys[4]), make_val(vals[4]));
    dtn_insert(dtn, make_key(keys[5]), make_val(vals[5]));
    dtn_insert(dtn, make_key(keys[6]), make_val(vals[6]));
    dtn_insert(dtn, make_key(keys[7]), make_val(vals[7]));
    //Kiem tra ham dtn_search()
    
    BT_val* val;
    val = dtn_search(dtn, make_key(keys[0]));
    if(val == NULL) {
        printf("Error: Khong tim thay A.\n");
        err_num++;
    }
    val = dtn_search(dtn, make_key(keys[2]));
    if(val == NULL) {
        printf("Error: Khong tim thay C.\n");
        err_num++;
    }
    val = dtn_search(dtn, make_key(keys[4]));
    if(val == NULL) {
        printf("Error: Khong tim thay E.\n");
        err_num++;
    }
    val = dtn_search(dtn, make_key(keys[6]));
    if(val == NULL) {
        printf("Error: Khong tim thay G.\n");
        err_num++;
    }
    val = dtn_search(dtn, make_key(keys[8]));
    if(val != NULL) {
        printf("Error: Tim thay M (khong co trong tu dien).\n");
        err_num++;
    }
    val = dtn_search(dtn, make_key(keys[9]));
    if(val != NULL) {
        printf("Error: Tim thay Sus (khong co trong tu dien).\n");
        err_num++;
    }

    //Kiem tra ham dtn_del
    BT_item* item;
    item = dtn_delete(dtn, make_key(keys[1]));
    if(item == NULL) {
        printf("Error: Khong xoa duoc B (Khong tim thay).\n");
        err_num++;
    }
    item = dtn_delete(dtn, make_key(keys[3]));
    if(item == NULL) {
        printf("Error: Khong xoa duoc D (Khong tim thay).\n");
        err_num++;
    }
    item = dtn_delete(dtn, make_key(keys[5]));
    if(item == NULL) {
        printf("Error: Khong xoa duoc F (Khong tim thay).\n");
        err_num++;
    }
    item = dtn_delete(dtn, make_key(keys[7]));
    if(item == NULL) {
        printf("Error: Khong xoa duoc H (Khong tim thay).\n");
        err_num++;
    }
    item = dtn_delete(dtn, make_key(keys[8]));
    if(item != NULL) {
        printf("Error: Xoa duoc M (khong co trong tu dien).\n");
        err_num++;
    }
    item = dtn_delete(dtn, make_key(keys[9]));
    if(item != NULL) {
        printf("Error: Xoa duoc Sus (khong co trong tu dien).\n");
        err_num++;
    }
    printf("Chay xong voi %d loi.\n", err_num);
}