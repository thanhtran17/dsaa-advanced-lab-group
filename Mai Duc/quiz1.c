#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"

void clear()
{    
  while ( getchar() != '\n' );
}

void jrb_print_db(JRB book);
void delete(JRB book);
void add_phone(JRB book, char *name, long phone);
void delete_phone(JRB book, char *name);
void modify_phone(JRB book, char *name);
void load_file(JRB book);
void update_file(JRB book);

//--------------------------------------------

int main(){
    JRB phonebook = make_jrb();
    int choice;
    char name[100]; long phone;

    do {
        printf("1. Print the phonebook.\n");
        printf("2. Add new phonenumber\n");
        printf("3. Modify existed phonenumber.\n");
        printf("4. Delete phonenumber.\n");
        printf("5. Delete all phonenumber. \n");
        printf("6. Exit.\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            jrb_print_db(phonebook);
            break;
        case 2:
            clear();
            printf("Input name: ");
            scanf("%s", name);
            printf("Input phonenumber: ");
            scanf("%ld", &phone);
            add_phone(phonebook, name, phone);
            break;
        case 3: 
            printf("Input name: ");
            scanf("%s", name);
            modify_phone(phonebook, name);
            break;
        case 4:
            printf("Input name: ");
            scanf("%s", name);
            delete_phone(phonebook, name);
            break;
        case 5:
            delete(phonebook);
        default:
            break;
        }
    } while (choice != 6);
    return 0;
}

//---------------------

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

void delete_phone(JRB book, char *name){
    JRB temp = jrb_find_str(book, name);
    if (temp != NULL) jrb_delete_node(temp);
    else printf("%s is not existed\n", name);
}

void modify_phone(JRB book, char *name){
    JRB temp = jrb_find_str(book, name);
    if (temp == NULL) printf("%s is not existed!\n", name);
    else {
        long phone;
        printf("Input new phonenumber: ");
        scanf("%ld", &phone);
        temp->val = new_jval_l(phone);
    }
}

void load_file(JRB book){
    FILE *f = fopen("phonebook.txt", "r");
    if (f == NULL){
        printf("Error!\n");
        return;
    }
    char *name;
    long phone;
    while (fscanf(f, "%s - %ld", name, &phone) != EOF) add_phone(book, name, phone);
    fclose(f);
}

void update_file(JRB book){
    FILE *f = fopen("phonebook.txt", "w");
    if (f == NULL){
        printf("Error!\n");
        return;
    }
    JRB temp = NULL;
    jrb_traverse(temp, book){
        fprintf(f, "%s - %ld", jval_s(temp->key), jval_l(temp->val));
    }
    fclose(f);
}
