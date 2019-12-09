#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binaryTree.h"

static PhoneBook trees[NUM_TREES];

void setupPhoneBook() {
    for (int i = 0; i < NUM_TREES; i++) {
    trees[i].size = 0;
    trees[i].type = i;
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
    
    char linebreak[] = "-------------------------------------";
    printf("%s\n\tName: %s %s\n\tNumber: %s\n%s\n", linebreak, record->value[0], record->value[1], record->value[2], linebreak); 
    printTree(record->right);
  }
}

void save(RecordPtr record, FILE *fptr) { // preorder traversal
  if (record != NULL) {
    for(int i = 0; i < 3; i++)
      fprintf(fptr, "%s ", record->value[i]);
    
    fprintf(fptr, "%c", '\n');
    
    save(record->left, fptr);
    save(record->right, fptr);
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

  char value[3][NAME_SIZE];

  while (fscanf(fptr, "%s", value[0]) == 1 &&
	 fscanf(fptr, "%s", value[1]) == 1 &&
	 fscanf(fptr, "%s", value[2]) == 1) {
  
    Record record;
    record.left = record.right = NULL;

    for (int i = 0; i < 3; i++)
      strcpy(record.value[i], value[i]);

    addRecord(record);
  }
  
  if (feof(fptr))
    printf("%s loaded successfully\n", filename);
  
  fclose(fptr);
}

void saveFile() {
  char filename[] = "records.txt";
  FILE *fptr = fopen(filename, "w");
  save(trees[0].rootRecord, fptr); // save by last name

  if (feof(fptr))
      printf("%s saved successfully\n", filename);
  
  fclose(fptr);
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
