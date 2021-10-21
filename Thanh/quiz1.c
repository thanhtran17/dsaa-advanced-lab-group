#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
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
int main()
{
  JRB phonebook, node;
  
  phonebook = make_jrb();
  jrb_insert_str(phonebook, "Thanh", new_jval_l(965));
  jrb_insert_str(phonebook, "Duc", new_jval_l(966));
  jrb_insert_str(phonebook, "Dang", new_jval_l(967));

  jrb_print_db(phonebook);

  return 0;
}