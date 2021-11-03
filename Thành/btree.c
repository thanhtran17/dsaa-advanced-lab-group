#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree/inc/btree.h"
#include <stdbool.h>
//--------------------------------------------------------
int insertPhoneData(BTA *root_t, char *name, char *phone, int *dsize)
{
  if (bfndky(root_t, name, dsize) == 0) {
    return btupd(root_t, name, phone, *dsize);
  }
  else {
    return btins(root_t, name, phone, *dsize);
  }
}
//--------------------------------------------------------
void findPhone(BTA *root_t, char *name, char *phone, int dsize, int *rsize)
{
  if (btsel(root_t, name, phone, dsize, rsize) == 0) {
    printf("Phone number found!: %s\n", phone);
  }
  else {
    printf("Can't find!\n");
  }
}
//--------------------------------------------------------
void deletePhone(BTA *root_t, char *name)
{
  if (btdel(root_t, name) == 0) {
    printf("Deleted %s\n", name);
  }
  else {
    printf("Name not found!\n");
  }
}
//--------------------------------------------------------
void printList(BTA* root_t, int pos, char *name, char *phone, int dsize, int *rsize)
{
  if (btpos(root_t, pos) != 0){
    printf("Position not found!\n");
  }
  while (btseln(root_t, name, phone, dsize, rsize) == 0){
    printf("  %s %s\n", name, phone);
  }
}
//--------------------------------------------------------
BTA *createBtree(BTA *root, char *pb)
{
  root = btopn(pb, 0 , 0);

  if (root == NULL){
    root = btcrt(pb, 0, 0);
  }

  return root;
}
//--------------------------------------------------------
int main()
{
  int choice, isSuccess = -1,
      rsize, dsize = 256;
  char phone[256], name[256];
  char pb[] = "phonebook.db";

  btinit();
  BTA* book;
  book = createBtree(book, pb);

  do {
    printf("----MENU----\n");
    printf("1. Insert entry to phonebook\n");
    printf("2. Find phone by name\n");
    printf("3. Delete entry\n");
    printf("4. Print phonebook\n");
    printf("5. End\n\n");
    
    printf("?? Enter your choice: ");
    scanf("%d", &choice);
    switch(choice) {
      case 1:
        printf("\nInsert entry to phonebook\n");
        scanf("%s", name);
        scanf("%s", phone);
        isSuccess = insertPhoneData(book, strdup(name), strdup(phone), &dsize);
        if (isSuccess != 0){
          printf("Insert failed!\n");
        }
        break;
      case 2:
        printf("\nInsert name to find phone number: ");
        scanf("%s", name);
        findPhone(book, strdup(name), strdup(phone), dsize, &rsize);
        break;
      case 3:
        printf("\nInsert name to delete entry: ");
        scanf("%s", name);
        deletePhone(book, name);
      case 4: 
        printList(book, 1, strdup(name), strdup(phone), dsize, &rsize);
        break;
      case 5:
        isSuccess = btcls(book);
        if (isSuccess == 0){
          printf("\nPhonebook closed\n");
          }
        else {
          printf("\nCan't close phonebook\n");
        }
        return 0;
    }
  }
  while (true);

  return 0;
}