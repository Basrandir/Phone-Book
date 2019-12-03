#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 20

typedef struct record {
  char value[3][NAME_SIZE];
  
  struct record *left;
  struct record *right;
} Record, *RecordPtr;

typedef struct {
  RecordPtr rootRecord;
  int size;
  int type;
} PhoneBook;

Record createRecord();
char* getAttribute();
char* getValue(char attribute[]);

void populateTree(PhoneBook phoneBook, RecordPtr record);
RecordPtr searchTree(PhoneBook phoneBook, char value[]);
void printTree(RecordPtr record);

void addRecord(Record record);
void retrieveRecord();
void deleteRecord();
void loadFile();
void saveFile();
void sortPhoneBook();

static PhoneBook byFirstName;
static PhoneBook byLastName;
static PhoneBook byPhoneNumber;

int main() {

  byFirstName.size = 0;
  byFirstName.type = 0;
  
  byLastName.size = 0;
  byLastName.type = 1;
  
  byPhoneNumber.size = 0;
  byPhoneNumber.type = 2;
  
  printf("\n\t\t\tPHONE BOOK\n\n");
  printf("--------------------------\n");
  printf("\t[1]\tAdd Contact\n");
  printf("\t[2]\tRetrieve Contact\n");
  printf("\t[3]\tDelete Contact\n");
  printf("\t[4]\tLoad Phone Book\n");
  printf("\t[5]\tSave Phone Book\n");
  printf("\t[6]\tSort Phone Book\n");
  printf("\t[7]\tExit\n");

  int option;
  scanf("%d", &option);

  switch(option) {
  case 1:
    addRecord(createRecord());
    break;
  case 2:
    retrieveRecord();
    break;
  case 3:
    deleteRecord();
    break;
  case 4:
    loadFile();
    break;
  case 5:
    saveFile();
    break;
  case 6:
    sortPhoneBook();
    break;
  case 7:
    exit(0);
  }
}

Record createRecord() {
  Record newRecord;

  printf("First Name: ");
  scanf("%s", newRecord.value[0]);

  printf("Last Name: ");
  scanf("%s", newRecord.value[1]);

  printf("Phone Number: ");
  scanf("%s", newRecord.value[2]);

  newRecord.left = newRecord.right = NULL;
  return newRecord;
}

char* getAttribute() {
  static char dataType[10];

  while(1) {
    printf("Data type: (first name/last name/number) ");
    scanf("%s", dataType);
    
    if(strcmp(dataType, "first name") || strcmp(dataType, "last name") || strcmp(dataType, "number")) {
      return dataType;
    }
  }
}

char* getValue(char attribute[]) {
  static char value[NAME_SIZE];
  
  printf("Enter %s: ", attribute);
  scanf("%s", value);

  return value;
}

void populateTree(PhoneBook phoneBook, RecordPtr record) {
  RecordPtr current = phoneBook.rootRecord;

  int type = phoneBook.type;
  int cmp;

  while((cmp = strcmp(record->value[type], current->value[type])) != 0) {
    if (cmp < 0) { // Left
      if (current->left == NULL) {
	current->left = record;
	break;
      } else
	current = current->left;
    } else { // Right
      if (current->right == NULL) {
	current->right = record;
	break;
      } else
	current = current->right;
    }
  }
}

RecordPtr searchTree(PhoneBook phoneBook, char value[]) {
  RecordPtr current = phoneBook.rootRecord;

  int type = phoneBook.type;
  
  while (current != NULL) {
    int cmp = strcmp(value, current->value[type]);
    
    if (cmp < 0) // Left
      current = current->left;
    else if (cmp > 0) // Right
      current = current->right;
    else
      return current;
  }

  return NULL;
}

void printTree(RecordPtr record) {
  if (record != NULL) {
    printTree(record->left);
    printf("Name: %s %s\nNumber: %s\n\n", record->value[0], record->value[1], record->value[2]);
    printTree(record->right);
  }
}

void addRecord(Record record) {
  Record records[2];

  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 3; j++) {
      strcpy(records[i].value[j], record.value[j]);
    }
  }
  
  populateTree(byFirstName, &record);
  populateTree(byLastName, &records[0]);
  populateTree(byPhoneNumber, &records[1]);
}

void retrieveRecord() {
  char* attribute = getAttribute();
  char* value = getValue(attribute);

  RecordPtr record;
  
  if (strcmp(attribute, "first name"))
    record = searchTree(byFirstName, value);
  else if (strcmp(attribute, "last name"))
    record = searchTree(byLastName, value);
  else
    record = searchTree(byPhoneNumber, value);

  printf("Name: %s %s\nNumber: %s", record->value[0], record->value[1], record->value[1]); 
}

void deleteRecord() {
  char* attribute = getAttribute();
  char* value = getValue(attribute);

  RecordPtr records[3];

  if (strcmp(attribute, "first name")) {
    records[0] = searchTree(byFirstName, value);
    records[1] = searchTree(byLastName, records[0]->value[1]);
    records[2] = searchTree(byPhoneNumber, records[0]->value[2]);
  } else if (strcmp(attribute, "last name")) {
    records[1] = searchTree(byLastName, value);
    records[0] = searchTree(byFirstName, records[1]->value[0]);
    records[2] = searchTree(byPhoneNumber, records[1]->value[2]);
  } else {
    records[2] = searchTree(byPhoneNumber, value);
    records[0] = searchTree(byFirstName, records[2]->value[0]);
    records[1] = searchTree(byLastName, records[2]->value[1]);
  }
}

void loadFile() {
  char filename[] = "records.txt";
  FILE *fptr = fopen(filename, "r");

  char values[3][NAME_SIZE];

  for (int i = 0; i < 3; i++, fscanf(fptr, "%s", values[i])) {
    RecordPtr newRecord = (RecordPtr) malloc(sizeof (Record));
    strcpy(newRecord->value[i], values[i]);

    populateTree(byFirstName, newRecord);
    populateTree(byLastName, newRecord);
    populateTree(byPhoneNumber, newRecord);
  }

  fclose(fptr);
}

void saveFile() {
  char filename[] = "records.txt";
  FILE *fptr = fopen(filename, "w");

  RecordPtr current = byLastName.rootRecord;
}

void sortPhoneBook() {
  char* attribute = getAttribute();

  if (strcmp(attribute, "first name"))
    printTree(byFirstName.rootRecord);
  else if (strcmp(attribute, "last name"))
    printTree(byLastName.rootRecord);
  else
    printTree(byPhoneNumber.rootRecord);
}
