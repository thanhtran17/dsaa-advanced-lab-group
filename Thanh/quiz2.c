#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
#define INITIAL_SIZE 10
#define INCREMENTAL_SIZE 5
//----------------------------------------------------------------------
typedef struct {
  char name[80];
  char number[11];
} PhoneEntry;
//
typedef struct {
  PhoneEntry* entries;
  int total;
  int size;
} PhoneBook;
//----------------------------------------------------------------------
PhoneBook* createPhoneBook();
void dropPhoneBook(PhoneBook* book);
void addPhoneNumber(char name[], char number[], PhoneBook* book);
PhoneEntry *getPhoneNumber(char* name, PhoneBook book);
void printPhoneBook(PhoneBook* book);
//----------------------------------------------------------------------
int main()
{
  char entryName[80], entryNumber[80];

  printf("--> New book created! \n");
  PhoneBook* book = createPhoneBook();
  printf("%d %d %d\n", book->entries, book->total, book->size);

  //
  printf("Entering new phone entry...\n");

  printf("  - Enter entry's name: ");
  scanf("%s", entryName);
  printf("  - Enter entry's number: ");
  scanf("%s", entryNumber);

  addPhoneNumber(entryName, entryNumber, book);

  //
  dropPhoneBook(book);
  printf("\n-->Book deleted!\n");
  printf("%d %d %d\n", book->entries, book->total, book->size);

  return 0;
}
//----------------------------------------------------------------------
PhoneBook *createPhoneBook()
{
  PhoneBook *newBook = (PhoneBook*) malloc(sizeof(PhoneBook));
  newBook->entries = (PhoneEntry*) malloc(sizeof(PhoneEntry) * INITIAL_SIZE);
  newBook->total = 0;
  newBook->size = INITIAL_SIZE;
}
//----------------------------------------------------------------------
void dropPhoneBook(PhoneBook* book)
{
  free(book->entries);
  free(book);
}
//----------------------------------------------------------------------
void addPhoneNumber(char* name, char* number, PhoneBook* book)
{
  // if the entry exists, the value should be overwritten

  //  

  strcpy(book->entries->name, name);
  strcpy(book->entries->number, number);

  printf("%s %s\n", book->entries->name, book->entries->number);

  if (book->size == book->total){
    book->entries = (PhoneEntry*) realloc(book->entries, sizeof(PhoneEntry) * (book->total + INCREMENTAL_SIZE));
    book->size += INCREMENTAL_SIZE;
  }
}
//----------------------------------------------------------------------
PhoneEntry *getPhoneNumber(char* name, PhoneBook book){

}
//----------------------------------------------------------------------
