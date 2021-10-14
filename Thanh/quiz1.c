#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
#define INITIAL_SIZE 10
#define INCREMENTAL_SIZE 5
//----------------------------------------------------------------------
typedef struct PhoneEntry{
  char name[80];
  char number[11];
} PhoneEntry[INITIAL_SIZE];
//
typedef struct PhoneBook {
  PhoneEntry* entries;
  int total;
  int size;
} PhoneBook;
//----------------------------------------------------------------------
PhoneBook* createPhoneBook();
void dropPhoneBook(PhoneBook* book);
void addPhoneNumber(char name[], char number[], PhoneBook* book);
void getPhoneNumber(char* name, PhoneBook* book);
void printPhoneBook(PhoneBook* book);
//----------------------------------------------------------------------
int main()
{
  char entryName[80], entryNumber[80];
  int choice;

  printf("--> New book created! \n");
  PhoneBook* book = createPhoneBook();
  printf("%d %d %d\n", book->entries, book->total, book->size);

  //
  while (true){
    char addContinue[4];

    printf("ENTERING NEW PHONE ENTRY...\n");
    printf("  - Enter entry's name: ");
    scanf("%s", entryName);
    printf("  - Enter entry's number: ");
    scanf("%s", entryNumber);
    addPhoneNumber(entryName, entryNumber, book);

    printf("?? Continue adding? (Yes/No): ");
    scanf("%s", addContinue);
    if (strcmp(addContinue, "No") == 0)
      break;
  }
  
  //
  while (true)
  {
    char enteredName[80];
    char findContinue[4];

    printf("\nENTER NAME TO GET PHONE NUMBER: ");
    scanf("%s", enteredName);
    getPhoneNumber(enteredName, book);

    printf("?? Continue finding? (Yes/No): ");
    scanf("%s", findContinue);
    if (strcmp(findContinue, "No") == 0)
      break;
  }

  //
  dropPhoneBook(book);
  printf("\n--> Book deleted!\n");

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
  (book->total)++;
  printf("Total book(s): %d\n", book->total);
  
  // if the entry exists, the value should be overwritten
  strcpy(book->entries[book->total]->name, name);
  strcpy(book->entries[book->total]->number, number);

  printf("Entry added: \n");

  if (book->size == book->total){
    book->entries = (PhoneEntry*) realloc(book->entries, sizeof(PhoneEntry) * (book->total + INCREMENTAL_SIZE));
    book->size += INCREMENTAL_SIZE;
  }

  printf("Print all entries' names: \n");
  for (int i = 1; i <= book->total; i++)
    printf("--> %s %s\n", book->entries[i]->name, book->entries[i]->number);
  printf("\n");
}
//----------------------------------------------------------------------
void getPhoneNumber(char* name, PhoneBook* book){
  for (int i = 1; i <= book->total; i++)
    if (strcmp(name, book->entries[i]->name) == 0){
      printf("--> Find: %s\n\n",(book->entries[i])->number);
      return ;
    }
  
  printf("--> Can't find.\n\n");
  return;
} 
//----------------------------------------------------------------------
