#include <stdio.h>

#include "binaryTree.h"

int main() {
  setupPhoneBook();

  int option;
  int result;
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

    while(1) {
      printf("> ");
      result = scanf("%d", &option);
      
      if(result != 1 || option < 1 || option > 7) {
	// Clears input buffer if user doesn't enter number
	char c;
	do {
	  scanf("%c", &c);
	} while(c != '\n');
	printf("Enter a number in the range of 1 to 7\n");
      } else
	break;
    }
    
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
      break;
    }
  }
}
