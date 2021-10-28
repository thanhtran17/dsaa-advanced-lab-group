#include <stdio.h>
#include "btree/inc/btree.h"

int main()
{
  char pb[] = "phonebook.db";

  btinit();
  BTA *book;

  book = btopn(pb, 0, 0);
  if (book == NULL)
    book = btcrt(pb, 0, 0);

  btcls(book);

  for (int i = 0; i < 10; i++){
    printf("DATA DATA\n");
  }

  return 0;
}