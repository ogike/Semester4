#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 99
#define FILE_NAME "data.txt"
#define FILE_NAME_TEMP "data_temp.txt"

#define NUM_OF_TERRITORIES 7
const char validTerritories[][BUFFER_SIZE] = {
    "Baratfa","Lovas","Szula", "Kigyos-patak",
    "Malom telek", "Paskom","Kaposztas kert"
};

void newPerson();
void modifyPerson();
int deletePerson();
void listPeople();

void chooseTerritory(char chosen[]); //handles territory validity
int isValidTerritory(char* terr);
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

    const char menuPrompt[] = "Choose a menu option (add: a, list: l, modify: m, delete: d, exit: x): ";

    //menu system
    printf(menuPrompt);
    while( fgets(menuString, BUFFER_SIZE, stdin) != NULL ){

        if(strlen(menuString) < 2){
            //if reading string with only \n
            continue;
        }

        if(strlen(menuString) != 2){ //curMenu can only be 1 char long
            fprintf(stderr, "Menu option (%s) can only be 1 character long.\n", menuString);
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
            case 'd':
                printf("Deleting person...\n");
                printf("Enter the name of the person to be deleted: ");
                char toDelete[BUFFER_SIZE];
                fgets(toDelete, BUFFER_SIZE, stdin);
                toDelete[strcspn(toDelete, "\r\n")] = 0;

                deletePerson(toDelete);
                break;  
            case 'm':
                printf("Modifying a person...\n");
                modifyPerson();
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

        printf(menuPrompt);
    }
    printf("At the end of the program... \n");
    exitProgram();
}

void newPerson(){
    printf("Give the details of the person to add:\n");

    char personName[BUFFER_SIZE];
    char personTerritory[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int personTryNum;

    printf("\tName: ");
    fgets(personName, BUFFER_SIZE, stdin);
    //clean the \n: the first occurence from the set of "\r\n" will be set to 0
    personName[strcspn(personName, "\r\n")] = 0; 

    printf("\tTerritory: ");
    chooseTerritory(personTerritory); //gets a valid territory from stdin into personTerritory

    printf("\tNumber of this try:");
    personTryNum = atoi( fgets(buffer, BUFFER_SIZE, stdin) );
    while(personTryNum == 0){
        fprintf(stderr, "Invalid try number, must be positive integer.\n");
        printf("\tNumber of this try:");
        personTryNum = atoi( fgets(buffer, BUFFER_SIZE, stdin) );
    }

    printf("Adding to database...\n");
    fseek(fp, 0, SEEK_END); //set the cursor to the end of the file
    fprintf(fp, "%s|%s|%d\n", personName, personTerritory, personTryNum);

    return;
}

//return 1: error/didnt find the person to delete
int deletePerson(char toDelete[]){
    FILE *ftemp = fopen(FILE_NAME_TEMP, "w");
    if(ftemp == NULL){
        fprintf(stderr, "Cant open temporary file for removing!");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    char * curName;


    fseek(fp, 0, SEEK_SET);
    int searching = 1;
    int curLine = 0;
    while(fgets(buffer, BUFFER_SIZE, fp) != NULL){
        char original[BUFFER_SIZE];
        strcpy(original, buffer);

        curName = strtok(buffer, "|");

        if( strcmp(curName, toDelete) == 0 ){
            //printf("debug Found person at line %d\n!", curLine);
            searching = 0;
            //skip writing
        } else{
            fprintf(ftemp, original);
        }
        curLine++;
    }

    if(searching == 1){
        fprintf(stderr, "Person with name (%s) not found!\n", toDelete);
        fclose(ftemp);
        return 1;
    } else{ //if we found and removed the searched person
        fclose(fp);
        fclose(ftemp);

        remove(FILE_NAME);
        rename(FILE_NAME_TEMP, FILE_NAME);

        fp = fopen(FILE_NAME, "r+");
        return 0;
    }
}

void modifyPerson(){
    printf("Enter the name of the person to be modified: ");
    char toModName[BUFFER_SIZE];
    fgets(toModName, BUFFER_SIZE, stdin);
    toModName[strcspn(toModName, "\r\n")] = 0;

    if( deletePerson(toModName) == 1 ){
        fprintf(stderr, "Couldnt find person to modify!\n");
        return;
    }

    char personTerritory[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int personTryNum;

    printf("\tNew territory: ");
    chooseTerritory(personTerritory); //gets a valid territory from stdin into personTerritory

    printf("\tNew number for this try:");
    personTryNum = atoi( fgets(buffer, BUFFER_SIZE, stdin) );
    while(personTryNum == 0){
        fprintf(stderr, "Invalid try number, must be positive integer.\n");
        printf("\tNumber of this try:");
        personTryNum = atoi( fgets(buffer, BUFFER_SIZE, stdin) );
    }

    printf("Adding to database...\n");
    fseek(fp, 0, SEEK_END); //set the cursor to the end of the file
    fprintf(fp, "%s|%s|%d\n", toModName, personTerritory, personTryNum);

    return;
}

void listPeople(){
    char buffer[BUFFER_SIZE];
    char territoryToList[BUFFER_SIZE];
    int listAll = -1;
    printf("List all the people or only a territory (a/t): ");
    while(listAll == -1){ 
        if( fgets(buffer, BUFFER_SIZE, stdin) != NULL){
            buffer[strcspn(buffer, "\r\n")] = 0; //strip newline 
            if(strcmp(buffer, "a") == 0){
                listAll = 1;
            } else if(strcmp(buffer, "t") == 0){
                listAll = 0;

                fprintf(stdout, "Which territory do you want to list?: ");
                chooseTerritory(territoryToList);

            } else{
                fprintf(stderr, "Not a valid option given (%s), write 'a' or 't': ", buffer);
            }
        }
    }

    fseek(fp, 0, SEEK_SET); //set the cursor to the beginning of the file
    while( fgets(buffer, BUFFER_SIZE, fp) != NULL ){ //reading all the lines
        if(listAll == 1){
            printf(buffer);
        } else{
            //get the second part of the line - the territory
            char original[BUFFER_SIZE];
            strcpy(original, buffer);
            char * territoryCur;
            territoryCur = strtok(buffer, "|");
            territoryCur = strtok(NULL, "|");   
            if(territoryCur == NULL){
                fprintf(stderr, "Wrong format in data file!");
                continue;
            }         

            if( strcmp(territoryCur, territoryToList) == 0 ){
                printf(original);
            }
        }
    }

} 

void chooseTerritory(char chosen[]){
    int isCorrect = 0;

    while (isCorrect == 0){
        if( fgets(chosen, BUFFER_SIZE, stdin) != NULL ){
            isCorrect = isValidTerritory(chosen);

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

//return 1 if terr is in the list of valid territories
//ignores trailing newline on terr
int isValidTerritory(char* terr){
    if(terr == NULL){
        fprintf(stderr, "Trying to validate empty string!");
        return 0;
    }
    int contains = 0;

    terr[strcspn(terr, "\r\n")] = 0; //strip newline 
    for(int i = 0; i < NUM_OF_TERRITORIES && contains == 0; i++){
        if(strcmp(terr, validTerritories[i]) == 0){
            contains = 1;
        }
    }

    return contains;
}

void exitProgram(){
    fclose(fp);
    exit(0);
}