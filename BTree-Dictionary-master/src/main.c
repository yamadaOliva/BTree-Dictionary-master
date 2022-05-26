#include <gtk/gtk.h>
#include <string.h>
#include "btree-5.0.1/inc/btree.h">
#include "dict/dict.h"
#include "dict/suggest.h"
#include "dict/tsv_reader.h"

#define TSV_DAT "dat/dict.tsv"

BT_dict dict;
BT_sndx sndx_tree;

GtkWidget *window_,*search_bar_,*no_button_,*yes_button_,*add_button_,*delete_button_,*replace_button_,*label_note_,*Some_Words,*Meaning,*status_label;
GtkTextBuffer *text_buffer_1,*text_buffer_2;
GObject *completion,*list_word;
const BTA *file_tree;
void read_file();
//api
void yes_button_set();
void no_button_set();
void add_button_set();
void delete_button_set();
void replace_button_set();
void search_bar_active();
static int status=0;
int check(char *text){
    int k = 0;
    int d = strlen(text);
    while (k < d)
    {
        if (!((text[k] == ' ') || (text[k] == '\n')))
        {
            return 0;
        }
        else
            k++;
    }
    return 1;
}
void out_main()
{
    dtn_free(dict);
    sndx_free(sndx_tree);
    gtk_main_quit();
}
int prefix(const char *string1, const char *string2){
	int length1 = strlen(string1);
	int length2 = strlen(string2);
	int index;

	if (length1 > length2)
		return 0;

	for (index = 0; index < length1; index++)
	{
		if (string1[index] != string2[index])
			return 0;
	}

	return 1;
}
void show_completion(char word[])
{
    GtkTreeIter Iter;
    gtk_list_store_clear(list_word);
    /*
    char tu_goi_y[30];
    int rsize;

    if (!bfndky(file_tree,word,&rsize)){
        gtk_list_store_append(list_word,&Iter);
        gtk_list_store_set(list_word,&Iter,0,word,-1);
    }

    int i,number=10,value;

    strcpy(tu_goi_y,word);
    for (i=0;i<number&&number<100;i++){
        if (!bnxtky(file_tree,tu_goi_y,&value)){
            if (prefix(word,tu_goi_y)){
                gtk_list_store_append(list_word,&Iter);
                gtk_list_store_set(list_word,&Iter,0,tu_goi_y,-1);
            }
            else number++;
        }
    }
    */
    BT_key key; key.s = word;
    int size = 0;
    BT_key* keys = dtn_suggest(dict, key, &size, 20);
    for (int i = 0; i < size; i++){
        gtk_list_store_append(list_word, &Iter);
        gtk_list_store_set(list_word, &Iter, 0, keys[i].s, -1);
    }
}

void next_word_guess(char word[])
{
    int n,size,val;
    char word_next[100];
    n=bfndky((BTA*)dict,word,&size);
    if (n==0){
        bnxtky((BTA*)dict,word_next,&size);
        gtk_entry_set_text(search_bar_,word_next);
        search_bar_active();
    }
    else{
        //btins((BTA*)dict,strdup(word),"",1);
        bnxtky((BTA*)dict,word_next,&size);
        gtk_entry_set_text(search_bar_,word_next);
        search_bar_active();
    }
}
void show_matching_soundex_word(char word[])
{
    char list_suggest_word[1000]="";
    /*
    char temp_word[100];
    int n=10,i,rsize;
    char *soundex_code=soundex(word);
    btins(file_tree,word,"",1);
    bfndky(file_tree,word,&rsize);
    int count=0;
    char temp_str[10][100];
    for (i=0;i<n;i++){
        if (bprvky(file_tree,temp_word,&rsize)==0){
            if (strcmp(soundex(temp_word),soundex_code)==0){
                strcpy(temp_str[count++],temp_word);
            }
        }
    }
    for (i=count-1;i>=0;i--){
        strcat(list_suggest_word,temp_str[i]);
        strcat(list_suggest_word,"\n");
    }
    bfndky(file_tree,word,&rsize);
    */
    BT_key key; key.s = word;
    int size = 0;
    BT_key* skeys = sndx_suggest(sndx_tree, key, &size, 5);
    for (int i=0; i < size;i++){ 
        char* temp_word = skeys[i].s;
        strcat(list_suggest_word,temp_word);
        strcat(list_suggest_word,"\n");
    }
    //In ra man hinh
    char output[1000]="Co phai ban dang tim: \n";
    strcat(output,list_suggest_word);
    gtk_text_buffer_set_text(text_buffer_1,output,-1);
}
gboolean search_entry_key_press( GtkEntry *g_search_entry, GdkEvent *event, gpointer none);

int main(int argc, char *argv[])
{
    btinit();
    dict = init_dtn();
    sndx_tree = init_sndx();

    dtn_read_tsv(dict, sndx_tree, TSV_DAT);

    GtkBuilder *builder;
    // creat menu
    gtk_init(&argc,&argv);
    builder =  gtk_builder_new_from_file("glade/ui_xin.glade");
    //
    status_label=GTK_WIDGET(gtk_builder_get_object(builder,"status_label"));
    window_    =GTK_WIDGET(gtk_builder_get_object(builder,"window_main"));
    search_bar_=GTK_WIDGET(gtk_builder_get_object(builder,"search_bar"));
    no_button_ =GTK_WIDGET(gtk_builder_get_object(builder,"no_button"));
    yes_button_=GTK_WIDGET(gtk_builder_get_object(builder,"yes_button"));
    add_button_=GTK_WIDGET(gtk_builder_get_object(builder,"add_button"));
    delete_button_=GTK_WIDGET(gtk_builder_get_object(builder,"delete_button"));
    replace_button_=GTK_WIDGET(gtk_builder_get_object(builder,"replace_button"));
    label_note_=GTK_WIDGET(gtk_builder_get_object(builder,"label_note"));
    Meaning = GTK_WIDGET(gtk_builder_get_object(builder,"Meaning"));
    text_buffer_1=GTK_TEXT_BUFFER(gtk_builder_get_object(builder,"textbuffer1"));
    Some_Words = GTK_WIDGET(gtk_builder_get_object(builder,"Some Words"));
    text_buffer_2=GTK_WIDGET(gtk_builder_get_object(builder,"textbuff2"));

    gtk_window_set_title(window_,"English dictionary");
     gtk_builder_connect_signals(builder, NULL);
     //
     completion = gtk_entry_completion_new();
    list_word = gtk_list_store_new(10, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(list_word));
    gtk_entry_set_completion(GTK_ENTRY(search_bar_), completion);
    // call back
    g_signal_connect(search_bar_, "key-press-event", G_CALLBACK(search_entry_key_press), NULL);
    g_signal_connect(search_bar_, "activate", G_CALLBACK(search_bar_active), NULL);
    g_signal_connect(add_button_,"clicked",G_CALLBACK(add_button_set),NULL);
    g_signal_connect(yes_button_,"clicked",G_CALLBACK(yes_button_set),NULL);
    g_signal_connect(no_button_,"clicked",G_CALLBACK(no_button_set),NULL);
    g_signal_connect(replace_button_,"clicked",G_CALLBACK(replace_button_set),NULL);
    g_signal_connect(delete_button_,"clicked",G_CALLBACK(delete_button_set),NULL);
     g_object_unref(builder);
    gtk_widget_show(window_);
    gtk_main();
    out_main();

    return 0;

}

void yes_button_set()   {
        int get_status;
        char*word,*meaning;
        GtkTextIter start;
        GtkTextIter end;
        switch(status)
        {
            case 1:
                word=gtk_label_get_text(GTK_LABEL(status_label));
                gtk_text_buffer_get_start_iter(text_buffer_1,&start);
                gtk_text_buffer_get_end_iter(text_buffer_1,&end);
                meaning = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(text_buffer_1),&start,&end,FALSE);
                
                BT_key add_key; add_key.s = word;
                BT_val add_val; add_val.s = meaning;
                get_status = dtn_insert(dict, add_key, add_val);
                sndx_insert(sndx_tree, add_key);
                
                if(check(meaning))
                {
                    gtk_label_set_label(GTK_LABEL(label_note_),"SUCCESS");
                    gtk_widget_set_visible(GTK_LABEL(label_note_),TRUE);

                    //dtn_insert(dict, key, val);
                    //btupd((BTA*)dict,strdup(word),strdup(meaning),strlen(meaning)+1);
                    return;
                }
                if(!get_status)
                {
                    gtk_label_set_label(GTK_LABEL(status_label),"SUCCESS");
                    gtk_widget_set_visible(GTK_LABEL(status_label), TRUE);
                    gtk_widget_set_visible(GTK_BUTTON(yes_button_), FALSE);
                    gtk_widget_set_visible(GTK_BUTTON(no_button_), FALSE);
                }else
                {
                    gtk_label_set_label(GTK_LABEL(status_label), "FAIL");
                    gtk_widget_set_visible(GTK_LABEL(status_label), TRUE);
                    gtk_widget_set_visible(GTK_BUTTON(yes_button_), FALSE);
                    gtk_widget_set_visible(GTK_BUTTON(no_button_), FALSE);
                }
                break;
            case 2:
                    word = gtk_entry_get_text(GTK_LABEL(search_bar_));
                    gtk_text_buffer_get_start_iter(text_buffer_1,&start);
                    gtk_text_buffer_get_end_iter(text_buffer_1,&end);
                    meaning = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(text_buffer_1),&start,&end,FALSE);
                    if(check(meaning))
                    {
                        gtk_label_set_label(GTK_LABEL(label_note_), "FAIL");
                        gtk_widget_set_visible(GTK_LABEL(label_note_), TRUE);
                        return;
                    }
                    get_status = btupd((BTA*)dict,strdup(word),strdup(meaning),strlen(meaning)+1);
                    if(!get_status)
                    {
                        gtk_label_set_label(GTK_LABEL(label_note_), "SUCCESS");
                        gtk_widget_set_visible(GTK_LABEL(label_note_), TRUE);
                        gtk_widget_set_visible(GTK_BUTTON(yes_button_), FALSE);
                        gtk_widget_set_visible(GTK_BUTTON(no_button_), FALSE);
                    }else
                    {
                        gtk_label_set_label(GTK_LABEL(label_note_), "FAIL");
                        gtk_widget_set_visible(GTK_LABEL(label_note_), TRUE);
                        gtk_widget_set_visible(GTK_BUTTON(yes_button_), FALSE);
                        gtk_widget_set_visible(GTK_BUTTON(no_button_), FALSE);
                    }
                break;
            case 3:
                    word = gtk_entry_get_text(GTK_LABEL(search_bar_));
                    BT_key key; key.s = strdup(word);
                    BT_item* item = dtn_delete(dict, key);
                    BT_item* sndx_item = sndx_delete(sndx_tree, key);
                    if(item != NULL)
                    {
                        gtk_label_set_label(GTK_LABEL(label_note_), "SUCCESS");
                        gtk_widget_set_visible(GTK_LABEL(label_note_), TRUE);
                        gtk_widget_set_visible(GTK_BUTTON(yes_button_), FALSE);
                        gtk_widget_set_visible(GTK_BUTTON(no_button_), FALSE);
                    }
                    else{
                        gtk_label_set_label(GTK_LABEL(label_note_), "FAIL");
                        gtk_widget_set_visible(GTK_LABEL(label_note_), TRUE);
                        gtk_widget_set_visible(GTK_BUTTON(yes_button_), FALSE);
                        gtk_widget_set_visible(GTK_BUTTON(no_button_), FALSE);
                    }
                break;
        }


    }
    void no_button_set()
    {
        gtk_label_set_text(GTK_LABEL(status_label),"");
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(text_buffer_1), "", -1);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(Meaning), FALSE);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(Meaning), FALSE);
        gtk_widget_set_visible(GTK_LABEL(label_note_),FALSE);
        gtk_widget_set_visible(GTK_BUTTON(yes_button_),FALSE);
        gtk_widget_set_visible(GTK_BUTTON(no_button_),FALSE);
    }
    void add_button_set()
    {
        char ptr[30];
        strcpy(ptr,gtk_entry_get_text(GTK_ENTRY(search_bar_)));
        gtk_label_set_text(GTK_LABEL(status_label),ptr);
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(text_buffer_1), "", -1);
        gtk_text_view_set_buffer(GTK_TEXT_VIEW(Meaning), GTK_TEXT_BUFFER(text_buffer_1));
        gtk_text_view_set_editable(GTK_TEXT_VIEW(Meaning), TRUE);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(Meaning), TRUE);
        gtk_label_set_text(GTK_LABEL(label_note_),"do you wanna to add this word");
        gtk_widget_set_visible(GTK_LABEL(label_note_),TRUE);
        gtk_widget_set_visible(GTK_BUTTON(yes_button_),TRUE);
        gtk_widget_set_visible(GTK_BUTTON(no_button_),TRUE);
        status=1;
    }
    void replace_button_set()
    {
        char  ptr[30];
        strcpy(ptr,gtk_entry_get_text(GTK_ENTRY(search_bar_)));
        gtk_label_set_text(GTK_LABEL(status_label),ptr);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(Meaning), TRUE);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(Meaning), TRUE);
        gtk_label_set_text(GTK_LABEL(label_note_),"Do you wanna to replace this word's meaning");
        gtk_widget_set_visible(GTK_LABEL(label_note_),TRUE);
        gtk_widget_set_visible(GTK_BUTTON(yes_button_),TRUE);
        gtk_widget_set_visible(GTK_BUTTON(no_button_),TRUE);
        status=2;
    }
     void delete_button_set()
    {
        gtk_label_set_text(GTK_LABEL(label_note_),"Do you wanna to delete this word?");
        gtk_widget_set_visible(GTK_LABEL(label_note_),TRUE);
        gtk_widget_set_visible(GTK_BUTTON(yes_button_),TRUE);
        gtk_widget_set_visible(GTK_BUTTON(no_button_),TRUE);
        status=3;
    }
    void search_bar_active()
    {
         char  word[100];
        char mean[10000]="";
        strcpy(word,gtk_entry_get_text(GTK_ENTRY(search_bar_)));
        printf("Word: %s\n",word);
        int size=0,n=0;
        BT_key key; key.s = strdup(word);
        BT_val* val = dtn_search(dict, key);
        //printf("n: %d\nsize: %d\n",n,size);
        //printf("Meaing: %s\n",mean);
        gtk_widget_set_visible(GTK_LABEL(label_note_),FALSE);
        gtk_widget_set_visible(GTK_BUTTON(yes_button_),FALSE);
        gtk_widget_set_visible(GTK_BUTTON(no_button_),FALSE);
        if (val != 0){
        //Neu tim thay tu
        gtk_label_set_text(GTK_LABEL(status_label),"FOUND");
        gtk_text_buffer_set_text(text_buffer_1,val->s,-1);
        gtk_text_view_set_buffer(GTK_TEXT_VIEW(Meaning), GTK_TEXT_BUFFER(text_buffer_1));
        gtk_text_view_set_editable(GTK_TEXT_VIEW(Meaning), FALSE);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(Meaning), FALSE);
        gtk_widget_set_sensitive(GTK_BUTTON(add_button_),FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(replace_button_), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(delete_button_), TRUE);
        //Hien thi cac tu gan giong
    }
    else {
        gtk_label_set_text(GTK_LABEL(status_label),word);
        show_matching_soundex_word(word);
        gtk_widget_set_sensitive(GTK_WIDGET(add_button_), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(replace_button_), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(delete_button_), FALSE);
        }
    }

gboolean search_entry_key_press( GtkEntry *g_search_entry, GdkEvent *event, gpointer none) {
    gtk_widget_set_sensitive(add_button_,FALSE);
    gtk_widget_set_sensitive(replace_button_,FALSE);
    gtk_widget_set_sensitive(delete_button_,FALSE);

    char word[50];
    GdkEventKey *key=(GdkEventKey *)event;
    int l;
    strcpy(word,gtk_entry_get_text(GTK_ENTRY(search_bar_)));
    l=strlen(word);
    if(key->keyval == GDK_KEY_Tab)
	{
		gtk_widget_grab_focus(replace_button_);
        next_word_guess(word);
	}
	else{
        if(key->keyval != GDK_KEY_BackSpace)
		{
			if((key->keyval != 65364)&&(key->keyval != 65362))
				word[l]=key->keyval;
			word[l+1]='\0';
		}
		else word[l-1]='\0';
		show_completion(word);
	}
	return FALSE;
}

