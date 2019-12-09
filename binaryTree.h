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

void setupPhoneBook();
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
