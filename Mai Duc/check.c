#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"

void jrb_print_db(JRB book){  //Print all the phonebook
    JRB a;
    jrb_traverse(a, book){
        printf("%s\t%ld\n", jval_s(a->key), jval_l(a->val));
    }
}

void delete(JRB book){  
    JRB temp;
    jrb_traverse(temp, book){
        free(jval_s(temp->key));
    }
    jrb_free_tree(book);
}

void add_phone(JRB book, char *name, long phone){    
    JRB temp = jrb_find_str(book, name);
    if (temp != NULL) jrb_delete_node(temp);
    jrb_insert_str(book, name, new_jval_l(phone));
}

int main(){
    char *name = "duc";
}