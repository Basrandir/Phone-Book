#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 20
#define NUM_TREES 3

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
char* getAttribute(char type[]);
char* getValue(char attribute[]);

void populateTree(PhoneBook phoneBook, RecordPtr record);
RecordPtr searchTree(PhoneBook phoneBook, char value[]);
void printTree(RecordPtr record);
RecordPtr delete(RecordPtr root, char value[], int type);
RecordPtr smallestValue(RecordPtr record);

void addRecord(Record record);
void retrieveRecord();
void deleteRecord();
void loadFile();
void saveFile();
void sortPhoneBook();

static PhoneBook trees[NUM_TREES];

int main() {

  // Setup the trees
  for (int i = 0; i < NUM_TREES; i++) {
    trees[i].size = 0;
    trees[i].type = i;
  }

  int cont = 1;
  while(cont) {
    printf("\tPHONE BOOK\n");
    printf("[1] Add Contact\n");
    printf("[2] Retrieve Contact\n");
    printf("[3] Delete Contact\n");
    printf("[4] Load Phone Book\n");
    printf("[5] Save Phone Book\n");
    printf("[6] Sort Phone Book\n");
    printf("[7] Exit\n");
    
    int option;
    printf("> ");
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
      cont = 0;
    }
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

char* getAttribute(char type[]) {
  int attribute;

  while(1) {
    printf("%s by:\n", type);
    printf("[1] First Name\n");
    printf("[2] Last Name\n");
    printf("[3] Phone Number\n");
    printf("> ");
    scanf("%d", &attribute);
    
    switch(attribute) {
    case 1:
      return "First Name";
    case 2:
      return "Last Name";
    case 3:
      return "Phone Number";
    default:
      printf("Incorrect Number\n");
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

  if(current != NULL) {

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
  } else
    trees[type].rootRecord = record;
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

void printTree(RecordPtr record) { // inorder traversal
  if (record != NULL) {
    printTree(record->left);
    printf("Name: %s %s\nNumber: %s\n\n", record->value[0], record->value[1], record->value[2]);
    printTree(record->right);
  }
}

RecordPtr delete(RecordPtr root, char value[], int type) {
  
  //searching for the item to be deleted
  if(root==NULL)
    return NULL;

  int cmp = strcmp(value, root->value[type]);
  
  if (cmp < 0)
    root->left = delete(root->left, value, type);
  else if(cmp > 0)
    root->right = delete(root->right, value, type);
  else {
    //No Children
    if(root->left == NULL && root->right == NULL) {
      free(root);
      return NULL;
    }

    //One Child
    else if(root->left == NULL || root->right == NULL) {
      RecordPtr temp;
      if(root->left == NULL)
	temp = root->right;
      else
	temp = root->left;
      free(root);
      return temp;
    }
  
    //Two Children
    else {
      RecordPtr temp = smallestValue(root->right);
      for(int i = 0; i < 3; i++)
	strcpy(root->value[i], temp->value[i]);
      root->right = delete(root->right, temp->value[type], type);
    }
  }  
  return root;
}

RecordPtr smallestValue(RecordPtr record) {
  while (record && record->left != NULL)
    record = record->left;

  return record;
}

void addRecord(Record record) {
  RecordPtr records[NUM_TREES];

  for(int i = 0; i < NUM_TREES; i++) {
    records[i] = (RecordPtr) malloc(sizeof (Record));
    for(int j = 0; j < 3; j++) {
      strcpy(records[i]->value[j], record.value[j]);
    }
  }

  for(int i = 0; i < NUM_TREES; i++) {
    populateTree(trees[i], records[i]);
  }
}

void retrieveRecord() {
  char* attribute = getAttribute("Search");
  char* value = getValue(attribute);

  RecordPtr record;

  if (strcmp(attribute, "First Name") == 0) {
    record = searchTree(trees[0], value);
  }
  else if (strcmp(attribute, "Last Name") == 0) {
    record = searchTree(trees[1], value);
  }
  else {
    record = searchTree(trees[2], value);
  }
  
  char linebreak[] = "-------------------------------------";
  printf("%s\n\tName: %s %s\n\tNumber: %s\n%s\n", linebreak, record->value[0], record->value[1], record->value[2], linebreak); 
}

void deleteRecord() {
  char* attribute = getAttribute("Delete");
  char* value = getValue(attribute);

  RecordPtr toBeDeleted;

  if(strcmp(attribute, "First Name") == 0)
    toBeDeleted = searchTree(trees[0], value);
  else if(strcmp(attribute, "Last Name") == 0)
    toBeDeleted = searchTree(trees[1], value);
  else
    toBeDeleted = searchTree(trees[2], value);
    
  for(int i = 0; i < NUM_TREES; i++)
    trees[i].rootRecord = delete(trees[i].rootRecord, toBeDeleted->value[i], i);
}

void loadFile() {
  char filename[] = "records.txt";
  FILE *fptr = fopen(filename, "r");

  char values[3][NAME_SIZE];

  for (int i = 0; i < 3; i++, fscanf(fptr, "%s", values[i])) {
    RecordPtr newRecord = (RecordPtr) malloc(sizeof (Record));
    strcpy(newRecord->value[i], values[i]);

    for (int i = 0; i < NUM_TREES; i++) 
      populateTree(trees[i], newRecord);
  }

  fclose(fptr);
}

void saveFile() {
  char filename[] = "records.txt";
  FILE *fptr = fopen(filename, "w");

  RecordPtr current = trees[1].rootRecord;
}

void sortPhoneBook() {
  char* attribute = getAttribute("Sort");

  if (strcmp(attribute, "First Name") == 0)
    printTree(trees[0].rootRecord);
  else if (strcmp(attribute, "Last Name") == 0)
    printTree(trees[1].rootRecord);
  else
    printTree(trees[2].rootRecord);
}
