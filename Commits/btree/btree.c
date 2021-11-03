#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree/inc/btree.h"

int insertPhoneData(BTA *head_node, char *name, char *phone, int *dsize)
{
  if (bfndky(head_node, name, dsize) == 0)
  {
    return btupd(head_node, name, phone, *dsize);
  }
  return btins(head_node, name, phone, *dsize);
}
void findPhone(BTA *head_node, char *name, char *phone, int dsize, int *rsize)
{
  if (btsel(head_node, name, phone, dsize, rsize) == 0)
  {
    printf("Finding successfully! Phone number is %s\n", phone);
  }
  else
  {
    printf("There is no name like that in list book. Please try again!\n");
  };
}
void deletePhone(BTA *head_node, char *name)
{
  if (btdel(head_node, name) == 0)
  {
    printf("Delete %s out of list successfully\n", name);
  }
  else
  {
    printf("There is no name like that in list book. Please try again!\n");
  }
}
void printList(BTA *head_node, int pos, char *name, char *phone, int dsize, int *rsize)
{
  if (btpos(head_node, pos) != 0)
  {
    printf("Some errors happened!\n");
  }
  while (btseln(head_node, name, phone, dsize, rsize) == 0)
  {
    printf("Name: %s Phone Number: %s\n", name, phone);
  }
}
BTA *createBtree(BTA *root, char *pb)
{
  root = btopn(pb, 0, 0);
  if (root == NULL)
  {
    root = btcrt(pb, 0, 0);
  }
  return root;
}
int main()
{
  char pb[] = "phonebook.db";
  char phone[256];
  char name[256];
  int choice;
  int rsize;       // current length of data
  int dsize = 256; // length of data
  int check_success = -1;

  // Initialize btree
  btinit();
  BTA *book;
  book = createBtree(book, pb);
  do
  {
    printf("1. Insert phone to phone book\n");
    printf("2. Find phone based on name\n");
    printf("3. Delete pairs of name, phone based on name\n");
    printf("4. Print List\n");
    printf("5. End\n");
    printf("Input your choice\n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
      printf("Input name and your phone number \n");
      scanf("%s", name);
      scanf("%s", phone);
      check_success = insertPhoneData(book, strdup(name), strdup(phone), &dsize);
      if (check_success != 0)
      {
        printf("Insert name and phone failed. Please try again!\n");
      }
      break;
    case 2:
      printf("Input name\n");
      scanf("%s", name);
      findPhone(book, strdup(name), strdup(phone), dsize, &rsize);
      break;
    case 3:
      printf("Input name\n");
      scanf("%s", name);
      deletePhone(book, name);
      break;
    case 4:
      printList(book, 1, strdup(name), strdup(phone), dsize, &rsize);
      break;
    case 5:
      check_success = btcls(book);
      if (check_success == 0)
      {
        printf("Close phone book successfully.\n");
      }
      else
      {
        printf("Close phone book failed.\n");
      }
      return 0;
    }
  } while (1);
  return 0;
}
