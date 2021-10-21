#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_SIZE 20
#define INCREMENTAL_SIZE 5
//--------------------------------------------------------------------------------------
typedef struct{
    void *key;
    void *value;
} Entry;

typedef struct{
  Entry *entries;
  int size, total;
  Entry (*makeNode)(void *, void *);
  int (*compare)(void *, void *);
} SymbolTable;

//--------------------------------------------------------------------------------------
Entry makePhone(void *name, void *phone)
{
  Entry res;
  res.key = strdup((char *)name);
  res.value = malloc(sizeof(long));
  memcpy(res.value, phone, sizeof(long));

  return res;
}

//--------------------------------------------------------------------------------------
int comparePhone(void *key1, void *key2)
{
  return strcmp((char *)key1, (char *)key2);
}
//--------------------------------------------------------------------------------------
SymbolTable *createSymbolTable(Entry makeNode(void *, void *), int compare(void *, void *))
{
  SymbolTable *newTable = (SymbolTable *)malloc(sizeof(SymbolTable));
  newTable->size = INITIAL_SIZE;
  newTable->total = 0;
  newTable->makeNode = makeNode;
  newTable->compare = compare;
  newTable->entries = (Entry *)malloc(sizeof(Entry) * newTable->size);
  
  return newTable;
}

//--------------------------------------------------------------------------------------
int binarySearch(Entry *entries, int l, int r, void *key)
{
  if (r >= l){
    int mid = l + (r - l) / 2;
    printf("%s %s\n", (char *)entries[mid].key, (char *)key);
    if (comparePhone(entries[mid].key, key) == 0)
      return mid;
    if (comparePhone(entries[mid].key, key) > 0)
      return binarySearch(entries, l, mid - 1, key);
    return binarySearch(entries, mid + 1, r, key);
  }
  
  return -1;
}

//--------------------------------------------------------------------------------------
void addEntry(void *key, void *value, SymbolTable *book)
{
  int pivot = binarySearch(book->entries, 0, book->total - 1, key);
  if (pivot == -1){
    if (book->total == book->size){
      book->size += INCREMENTAL_SIZE;
      book->entries = (Entry *)realloc(book->entries, sizeof(Entry) * book->size);
    }
    book->entries[book->total++] = makePhone(key, value);
  }
  else{
    memcpy(book->entries[pivot].value, value, sizeof(long));
  }
}
//--------------------------------------------------------------------------------------
void dropSymbolTable(SymbolTable *book)
{
  free(book->entries);
  free(book);
}
//--------------------------------------------------------------------------------------
Entry *getEntry(void *key, SymbolTable *book)
{
  for (int i = 0; i < book->total; i++){
    if (strcmp(book->entries[i].key, key) == 0)
      return &(book->entries[i]);
  }
  
  return NULL;
}
//--------------------------------------------------------------------------------------
void printAll(SymbolTable *book)
{
  for (int i = 0; i < book->total; i++)
    printf("%s %ld\n", (char *)(book->entries[i]).key, *(long *)book->entries[i].value);
}
//--------------------------------------------------------------------------------------
int main()
{
  SymbolTable *table = createSymbolTable(makePhone, comparePhone);
  int choice;
  char name[100];
  long num;
  while (1){
    printf("------MENU------\n");
    printf("1. Add entry to table \n");
    printf("2. Print table \n");
    printf("3. Get detail entry \n");
    printf("4. Delete table\n");
    printf("5. Exit\n");
    printf("----------------\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice){
      case 1:
        printf("Input name: ");
        scanf("%s", name);
        printf("Input phone number: ");
        scanf("%ld", &num);
        addEntry(name, &num, table);
        break;
      case 2:
        printAll(table);
        break;
      case 3:
        printf("Input name: \n");
        scanf("%s", name);
        Entry *needEntry = getEntry(name, table);
        printf("Found object : %s %d\n", (char *)needEntry->key, *(long *)needEntry->value);
        break;
      case 4:
        dropSymbolTable(table);
        return 0;
      case 5:
        return 0;
    }
  }
}
//--------------------------------------------------------------------------------------