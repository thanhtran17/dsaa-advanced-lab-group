#include <stdio.h>
#include "jrb.h"
#include "jval.h"
#include <string.h>
#include <stdlib.h>
//----------------------------------------------------------
JRB find_number(JRB head_node, long number)
{
  JRB tmp;
  jrb_traverse(tmp, head_node)
  {
    if (jval_l(tmp->val) == number)
    {
      return tmp;
    }
  }
  return NULL;
}
//----------------------------------------------------------
JRB find_name(JRB head_node, char *name)
{
  JRB tmp;
  jrb_traverse(tmp, head_node)
  {
    if (strcmp(jval_s(tmp->key), name) == 0)
    {
      return tmp;
    }
  }
  return NULL;
}
//----------------------------------------------------------
void modifyNumber(JRB new_node, long number)
{
  JRB node = new_node;
  node->val = new_jval_l(number);
}
//----------------------------------------------------------
void add_phone_number(JRB *head_node, char *name, long number)
{
  if (find_name(*head_node, name))
  {
    modifyNumber(find_name(*head_node, name), number);
  }
  else
  {
    jrb_insert_str(*head_node, name, new_jval_l(number));
  }
}
//----------------------------------------------------------
void jrb_print_db(JRB head_node)
{
  JRB tmp;
  jrb_rtraverse(tmp, head_node)
  {
    printf("Name: %s - Phone number: %ld\n", jval_s(tmp->key), jval_l(tmp->val));
  }
}
//----------------------------------------------------------
void delete (JRB head_node)
{
  JRB temp;
  jrb_traverse(temp, head_node)
  {
    free(jval_s(temp->key));
  }
  jrb_free_tree(head_node);
}
//----------------------------------------------------------
void deleteName(JRB *new_node)
{
  jrb_delete_node(*new_node);
}
//----------------------------------------------------------
void load_file(JRB head_node)
{
  FILE *fp;
  char name[100];
  long number;
  if ((fp = fopen("test.txt", "r")) == NULL)
  {
    printf("Error! opening file");
    exit(1);
  }
  while (!feof(fp))
  {
    fscanf(fp, "%s %ld", name, &number);
    add_phone_number(&head_node, strdup(name), number);
  }
  jrb_print_db(head_node);
  fclose(fp);
}
//----------------------------------------------------------
int main()
{
  JRB head_node = make_jrb();
  int choice;
  char name[100];
  long number;

  while (1)
  {
    printf("1. Add phone number\n");
    printf("2. Modify number\n");
    printf("3. Delete a name based on number\n");
    printf("4. Print data\n");
    printf("5. Read data from file\n");
    printf("6. Feel bored? Get out\n");
    printf("Input your choice: \n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
      printf("Input name and number: \n");
      scanf("%s %ld", name, &number);
      add_phone_number(&head_node, strdup(name), number);
      break;
    case 4:
      jrb_print_db(head_node);
      break;
    case 2:
      printf("Input name: \n");
      scanf("%s", name);
      JRB change_number = find_name(head_node, name);
      printf("Input new number for changing: \n");
      scanf("%ld", &number);
      modifyNumber(change_number, number);
      jrb_print_db(head_node);
      break;
    case 3:
      printf("Input number: \n");
      scanf("%ld", &number);
      JRB change_numbers = find_number(head_node, number);
      deleteName(&change_numbers);
      jrb_print_db(head_node);
      break;
    case 5:
      load_file(head_node);
      break;
    case 6:
      delete (head_node);
      return 0;
    }
  };
}
//----------------------------------------------------------

