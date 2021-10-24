#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
//------------------------------------------------------------
JRB jrb_find_key(JRB tree, Jval value)
{
  JRB temp;
  jrb_traverse(temp, tree){
    if (jval_l(temp->val) == jval_l(value))
      return temp;
  }

  return NULL;
}
//------------------------------------------------------------
void delete_node(JRB tree, char* key)
{
  JRB temp = jrb_find_str(tree, key);

  if (temp != NULL){
    jrb_delete_node(temp);
  }
  else{
    printf("Key invalid! \n");
  } 
}

//------------------------------------------------------------
void jrb_print_db(JRB tree) 
{
  JRB temp;
  jrb_traverse(temp, tree) {
    printf("  - Name: %s; ", jval_s(temp->key));
    printf("Phone numbers: %ld", jval_l(temp->val));
    printf("\n");
  }
}

//------------------------------------------------------------
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
//------------------------------------------------------------
int main()
{
  JRB phonebook, node;
  
  phonebook = make_jrb();

  jrb_insert_str(phonebook, "Thanh", new_jval_l(965));
  jrb_insert_str(phonebook, "Duc", new_jval_l(966));
  jrb_insert_str(phonebook, "Khue", new_jval_l(9617));
  jrb_insert_str(phonebook, "Dang", new_jval_l(967));
  jrb_insert_str(phonebook, "Trinh", new_jval_l(96127));

  jrb_print_db(phonebook);
  printf("\n");

  printf("Find phone by name: \n");
  printf("  - (Thanh) %d\n", jrb_find_str(phonebook, "Thanh")->val);
  printf("  - (Duc) %d\n", jrb_find_str(phonebook, "Duc")->val);
  printf("\n");

  printf("Find name by phone number: \n");
  printf("(966) %s\n", jrb_find_key(phonebook, (Jval) 966)->key);
  printf("(96127) %s\n", jrb_find_key(phonebook, (Jval) 96127)->key);
  printf("\n");
  
  printf("Modify: \n");
  modify_phone(phonebook, "Duc");
  printf("\n");

  printf("Delete a node: \n");
  delete_node(phonebook, "Thanh");
  jrb_print_db(phonebook);
  printf("\n");


  printf("Free phonebook tree\n");
  jrb_free_tree(phonebook);
  printf("--> Deleted!\n");
  printf("\n");
  
  return 0;
}