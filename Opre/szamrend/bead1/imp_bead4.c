#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STARTING_SIZE 2
#define INCREMENT_SIZE 5
#define MAX_NAME_LENGTH 30
#define MAX_LINE_LENGTH 99 //how many characters can the user possibly enter in a line, even if the input is invalid (used for fget)

struct Person
{
  char name[MAX_NAME_LENGTH];
  unsigned age;
  char job[MAX_NAME_LENGTH];
};

//Based on the assigment description, "Fontos: A program magától ne írjon ki semmit a terminálra a 7. pontban kért hibaüzeneten kívül!"
//If this is set to one, nothing will be written to the console apart from the list function, and the errors
//if set to 0, the program will be user-friendly, and easier to debug
const int strictOutput = 1;

void addPerson();
void deletePerson();
void growDatabase();
void reduceDatabase();
void listDatabase();
void exitProgram();
void freeMemory();
void allocError();

struct Person** iterator;
int size;
int count;

int main(void) {
  char curMenu;
  char menuString[MAX_LINE_LENGTH];

  iterator = (struct Person **)malloc(STARTING_SIZE * sizeof(struct Person*));
  if (iterator == NULL)
    allocError();

  size = STARTING_SIZE;
  count = 0;

  if(!strictOutput)
    printf("\nChoose an option in the menu:");

  while (fgets(menuString, MAX_LINE_LENGTH, stdin) != NULL)
  {
    curMenu = menuString[0];
    if (strlen(menuString) == 2)
    {
      switch (curMenu)
      {
        case 'a':
          if(!strictOutput)
            printf("Adding persons name, age, job\n");
          addPerson();
          break;
        case 'l':
          if(!strictOutput)
            printf("Listing database, curCount: %d", count);
          listDatabase();
          break;
        case 'd':
          deletePerson();
          break;
        case 'x':
          exitProgram();
          break;
        default:
          fprintf(stderr,"Invalid menu option, option not found (avaliable options: a/l/x/d)\n");
          break;
      }
    }
    else
    {
      fprintf(stderr,"Invalid menu option, must be 1 character long\n");
    }
    if(!strictOutput)
      printf("\nChoose anoter option in the menu");
  }
  exitProgram(); //happens if eof is read, my interpretation is that if reading inputs from a file, the program should exit at the end of it

  return 0;
}

void addPerson ()
{
  struct Person *tempPerson;
  char tempString[MAX_LINE_LENGTH];
  int tempLength;

  tempPerson = (struct Person*)malloc(sizeof(struct Person));
  if(tempPerson)
  {
    //Getting name
    if(!strictOutput)
      printf("Whats's the name of this person? ");
    fgets(tempString, MAX_LINE_LENGTH, stdin);
    tempLength = strlen(tempString);
    if(tempLength > MAX_NAME_LENGTH)
    {
      fprintf(stderr, "Given name is too long, must be shorter than %d characters, going back to menu\n", MAX_NAME_LENGTH);
      free(tempPerson);
      return;
    }
    strncpy(tempPerson->name, tempString, tempLength-1);
    tempPerson->name[tempLength-1] = '\0';

    //Getting age
    if(!strictOutput)
      printf("Whats's the age of this person? ");
    fgets(tempString, MAX_LINE_LENGTH, stdin);
    tempPerson->age = atoi(tempString);
    if(tempPerson->age == 0)
    {
      fprintf(stderr, "Given age is invalid, going back to menu\n");
      free(tempPerson);
      return;
    }

    //Getting job
    if(!strictOutput)
      printf("Whats's the job of this person? ");
    fgets(tempString, MAX_LINE_LENGTH, stdin);
    tempLength = strlen(tempString);
    if(tempLength > MAX_NAME_LENGTH)
    {
      fprintf(stderr, "Given name is too long, must be shorter than %d characters, going back to menu\n", MAX_NAME_LENGTH);
      free(tempPerson);
      return;
    }
    strncpy(tempPerson->job, tempString, tempLength-1);
    tempPerson->job[tempLength-1] = '\0';

    if(!strictOutput)
      printf("Saved name: %s, age: %u, job: %s", tempPerson->name, tempPerson->age, tempPerson->job);

    if (count >= size)
      growDatabase();

    iterator[count] = tempPerson;
    count++;
  }
  else
    allocError();
}

void growDatabase ()
{
  size += INCREMENT_SIZE;
  if(!strictOutput)
    printf("Growing database, new size: %d\n", size);
  iterator = (struct Person**)realloc(iterator, size * sizeof(struct Person*));
  if (iterator == NULL)
    allocError();
}

void listDatabase()
{
  printf("Listing database, with size: %d, count of elements: %d", size, count);
  for(int i = 0; i < count; i++)
  {
    printf("\nPerson %d's: name: %s, age: %u, job: %s", i+1, iterator[i]->name, iterator[i]->age, iterator[i]->job);
  }

  //so the new menu point will be written into a new line
  if(strictOutput)
   printf("\n");
}

void deletePerson ()
{
  char tempString[MAX_LINE_LENGTH];
  char nameToDel[MAX_NAME_LENGTH];
  int tempLength;
  if(!strictOutput)
    printf("Whats's the name of the person you want to delete? ");
  fgets(tempString, MAX_LINE_LENGTH, stdin);
  tempLength = strlen(tempString);
  if(tempLength > MAX_NAME_LENGTH)
  {
    fprintf(stderr, "Given name is too long, must be shorter than %d characters, going back to menu\n", MAX_NAME_LENGTH);
    return;
  }
  strncpy(nameToDel, tempString, tempLength-1);
  nameToDel[tempLength-1] = '\0';

  int i = 0;
  while (i < count && strcmp(iterator[i]->name, nameToDel) != 0)
    i++;
  
  if(i < count)
  {
    if (i < count-1) //if the last person is to be deleted, we dont need to copy anything
    {
      strcpy(iterator[i]->name, iterator[count-1]->name);
      iterator[i]->age = iterator[count-1]->age;
      strcpy(iterator[i]->job, iterator[count-1]->job);
    }
    free(iterator[count-1]);
    count--;
    if(count <= size-(INCREMENT_SIZE+1))
      reduceDatabase();
  }
  else
   fprintf(stderr, "The specified name wasnt found, going back to menu\n");  
}

void reduceDatabase ()
{
  size -= INCREMENT_SIZE;
  
  if(!strictOutput)
    printf("Reducing database, new size: %d\n", size);

  iterator = (struct Person**)realloc(iterator, size * sizeof(struct Person*));
  if (iterator == NULL)
    allocError();
}


void exitProgram ()
{
  freeMemory();
  exit (0);
}

void freeMemory()
{
  for(int i = 0; i < count; i++)
  {
    free(iterator[i]);
  }
  free(iterator);
}

void allocError ()
{
  fprintf(stderr, "Not enough memory, exiting program");
  exit(1);
}
