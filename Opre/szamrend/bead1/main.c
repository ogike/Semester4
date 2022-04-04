#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 99
#define FILE_NAME "data.txt"

#define NUM_OF_TERRITORIES 7
const char validTerritories[][BUFFER_SIZE] = {
    "Baratfa","Lovas","Szula", "Kigyos-patak",
    "Malom telek", "Paskom","Kaposztas kert"
};

void newPerson();
void modifyPerson();
void deletePerson();
void listPeople();

void chooseTerritory(char chosen[]); //handles territory validity
int contains(char* key, char** set, int setNum);
int searchLine(); //???
void exitProgram();

FILE *fp;

int main(void){
    char curMenu;
    char menuString[BUFFER_SIZE];

    //opening database
    fp = fopen(FILE_NAME, "r+");
    if(fp == NULL){
        fprintf(stderr, "Can't open data.txt file.");
        exit(1);
    }

    //menu system
    printf("Choose a menu option (add: a, list: l, exit: x): ");
    while( fgets(menuString, BUFFER_SIZE, stdin) != NULL ){

        if(strlen(menuString) < 2){
            //if reading string with only \n
            //TODO: why is this buggy?
            continue;
        }

        if(strlen(menuString) != 2){ //curMenu can only be 1 char long
            fprintf(stderr, "Menu option (%s) can only be 1 character long.", menuString);
        }

        else {
            curMenu = menuString[0];

            switch (curMenu)
            {
            case 'a':
                printf("Adding new person:\n");
                newPerson();
                break;
            
            case 'l':
                printf("Listing people...\n");
                listPeople();
                break;
            
            case 'x':
                printf("Exiting program... \n");
                exitProgram();
                break;

            default:
                fprintf(stderr,"Invalid menu option\n");
                break;
            }

        }

        printf("Choose a menu option (add: a, list: l, exit: x): ");
    }
    printf("At the end of the program... \n");
    exitProgram();
}

void newPerson(){
    printf("Give the details of the person to add:\n");

    char personName[BUFFER_SIZE];
    char personTerritory[BUFFER_SIZE];
    int personTryNum;

    printf("\tName: ");
    fgets(personName, BUFFER_SIZE, stdin);
    //clean the \n: the first occurence from the set of "\r\n" will be set to 0
    personName[strcspn(personName, "\r\n")] = 0; 

    printf("\tTerritory: ");
    chooseTerritory(personTerritory); //gets a valid territory from stdin into personTerritory
    // fgets(personTerritory, BUFFER_SIZE, stdin);
    // personTerritory[strcspn(personTerritory, "\r\n")] = 0; 

    printf("\tNumber of this try:");
    //TODO: not good with txt input
    scanf("%d", &personTryNum);

    printf("Adding to database...\n");
    fseek(fp, 0, SEEK_END); //set the cursor to the end of the file
    // printf("\tseek done\n");
    fprintf(fp, "%s|%s|%d\n", personName, personTerritory, personTryNum);
    // printf("\twriting done\n");

    return;
}

void listPeople(){
    char buffer[BUFFER_SIZE];
    char territoryToList[BUFFER_SIZE];
    int listAll = -1;
    printf("List all the people or only a territory (a/t): ");
    while(listAll == -1){ 
        if( fgets(buffer, BUFFER_SIZE, stdin) != NULL){
            if(strcmp(buffer, "a") == 0){
                listAll = 1;
            } else if(strcmp(buffer, "t") == 0){
                listAll = 0;

                fprintf(stdout, "Which territory do you want to list?: ");
                chooseTerritory(territoryToList);

            } else{
                //TODO: gets stuck here 
                fprintf(stderr, "Not a valid option given, write 'a' or 't': ");
            }
        }
    }

    fseek(fp, 0, SEEK_SET); //set the cursor to the beginning of the file
    while( fgets(buffer, BUFFER_SIZE, fp) != NULL ){ //reading all the lines
        if(listAll == 1){
            printf(buffer);
        } else{
            //get the second part of the line - the territory
            char * territoryCur;
            territoryCur = strtok(buffer, "|");
            territoryCur = strtok(NULL, "|");

            if( contains(territoryCur, validTerritories, NUM_OF_TERRITORIES) == 1 ){
                printf(buffer);
            }
        }
    }

} 

void chooseTerritory(char chosen[]){
    int isCorrect = 0;

    while (isCorrect == 0){
        if( fgets(chosen, BUFFER_SIZE, stdin) != NULL ){
            isCorrect = contains(chosen, validTerritories, NUM_OF_TERRITORIES);

            // chosen[strcspn(chosen, "\r\n")] = 0; //strip newline 
            // for(int i = 0; i < len && isCorrect == 0; i++){
            //     if(strcmp(chosen, validTerritories[i]) == 0){
            //         isCorrect = 1;
            //     }
            // }
        }
        
        if(isCorrect == 0){
            fprintf(stderr, "Incorrect territory given.");
            fprintf(stdout, " List of territories: \n");
            for(int i = 0; i < NUM_OF_TERRITORIES; i++){
                fprintf(stdout, "\t%s\n", validTerritories[i]);
            }

            fprintf(stdout, "Give a valid territory: ");
        }
    } 
}

//return 1 if key is in the list of strings(set)
//ignores trailing newline on key
int contains(char* key, char** set, int setNum){
    int contains = 0;

    key[strcspn(key, "\r\n")] = 0; //strip newline 
    for(int i = 0; i < setNum && contains == 0; i++){
        if(strcmp(key, set[i]) == 0){
            contains = 1;
        }
    }

    return contains;
}

void exitProgram(){
    fclose(fp);
    exit(0);
}