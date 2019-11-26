#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void retrieveRecord(PhoneBook phoneBook);
void deleteRecord(PhoneBook phoneBook);
void loadFile(PhoneBook phoneBook);
void saveFile(PhoneBook phoneBook);
void sortPhoneBook(PhoneBook phoneBook);

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
    retrieveRecord(phoneBook);
    break;
  case 3:
    deleteRecord(phoneBook);
    break;
  case 4:
    loadFile(phoneBook);
    break;
  case 5:
    saveFile(phoneBook);
    break;
  case 6:
    sortPhoneBook(phoneBook);
    break;
  case 7:
    exit(0);
  }
}

RecordPtr createRecord() {
  RecordPtr newRecord = (RecordPtr) malloc(sizeof (Record));

  printf("First Name: ");
  scanf("%s", newRecord->firstName);

  printf("Last Name: ");
  scanf("%s", newRecord->lastName);

  printf("Phone Number: ");
  scanf("%d", &(newRecord->number));

  return newRecord;
}

enum attribute getAttribute() {
  char dataType[10];

  while(1) {
    printf("Data type: (first name/last name/number) ");
    scanf("%s", dataType);
    
    if(strcmp(dataType, "first name")) {
      return FirstName;
    } else if(strcmp(dataType, "last name")) {
      return LastName;
    } else if(strcmp(dataType, "number")) {
      return PhoneNumber;
    }
  }
}

char* getValue() {
  static char value[NAME_SIZE];
  
  printf("Enter Value: ");
  scanf("%s", value);

  return value;
}
