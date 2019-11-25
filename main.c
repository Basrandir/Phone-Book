#include <stdio.h>
#include <stdlib.h>

#define NAME_SIZE 20

typedef struct record {
  char firstName[NAME_SIZE];
  char lastName[NAME_SIZE];
  int number;
  struct record *left;
  struct record *right;
} Record, *RecordPtr;

typedef struct {
  RecordPtr rootRecord;
  int size;
} PhoneBook;

enum attribute { FirstName, LastName, PhoneNumber };

RecordPtr createRecord();
enum attribute getAttribute();
char* getValue();

void addRecord(PhoneBook phoneBook, RecordPtr record);
void retrieveRecord(PhoneBook phoneBook, enum attribute attr, char value[]);
void deleteRecord(PhoneBook phoneBook, enum attribute attr, char value[]);
void loadFile(PhoneBook phoneBook);
void saveFile(PhoneBook phoneBook);
void sortPhoneBook(PhoneBook phoneBook, enum attribute attr);

int main() {

  PhoneBook phoneBook;
  phoneBook.size = 0;
  
  printf("\tPHONE BOOK\n");
  printf("--------------------------\n");
  printf("1. Add Contact\n");
  printf("2. Retrieve Contact\n");
  printf("3. Delete Contact\n");
  printf("4. Load Phone Book\n");
  printf("5. Save Phone Book\n");
  printf("6. Sort Phone Book\n");
  printf("7. Exit\n");

  int option;
  scanf("%d", &option);

  switch(option) {
  case 1:
    addRecord(phoneBook, createRecord());
    break;
  case 2:
    retrieveRecord(phoneBook, getAttribute(), getValue());
    break;
  case 3:
    deleteRecord(phoneBook, getAttribute(), getValue());
    break;
  case 4:
    loadFile(phoneBook);
    break;
  case 5:
    saveFile(phoneBook);
    break;
  case 6:
    sortPhoneBook(phoneBook, getAttribute());
    break;
  case 7:
    exit(0);
  }
}
