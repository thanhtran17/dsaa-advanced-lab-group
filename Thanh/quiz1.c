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

//-------------------------------------------------------------------
void jrb_print_db(JRB book)
{
    JRB temp;
  
    jrb_traverse(temp, book){
        printf("%s\t%ld\n", jval_s(temp->key), jval_l(temp->val));
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
  int choice;
  char new_name[100], name_to_find[100], temp[100];
  long new_num;

  phonebook = make_jrb();

  while (1) {
    printf("------MENU------\n");
    printf("1. Add entry to table \n");
    printf("2. Print table \n");
    printf("3. Modify phone number \n");
    printf("4. Find phone by name\n");
    printf("5. Fine name by phone number\n");
    printf("6. Delete\n");
    printf("7. Exit\n");
    printf("----------------\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
      printf("Input name: ");
      printf("%s\n", new_name);

      scanf("%s", new_name);

      printf("%s\n", new_name);
      strcpy(temp, new_name);
      printf("Input phone number: ");
      scanf("%ld", &new_num);
      jrb_insert_str(phonebook, temp, new_jval_l(new_num));
      printf("%s\n", new_name);
      break;
    case 2:
      jrb_print_db(phonebook);
      break;
    case 3:
      printf("Enter name to modify number:");
      scanf("%s", name_to_find);
      modify_phone(phonebook, name_to_find);
      break;
    default:
      return 1;
      break;
    }
  }


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