#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for pipe()
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#define BUFFER_SIZE 99
#define FILE_NAME "data.txt"
#define FILE_NAME_TEMP "data_temp.txt"

#define NUM_OF_TERRITORIES 7
#define RACE_TERRITORIES_2_START_IND 4
const char validTerritories[][BUFFER_SIZE] = {
    "Baratfa","Lovas", "Kigyos-patak", "Kaposztas kert", //race territory 1
    "Szula", "Malom telek", "Paskom"                     //race territory 2
};

void startRace();
void raceManageTerritory(int writeFd, int readFd);

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

    const char menuPrompt[] = "Choose a menu option (add: a, list: l, modify: m, delete: d, exit: x, start race: r): ";

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
            case 'r':
                printf("Starting race... \n");
                startRace();
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

void startRace(){
    int terr1PipeToChildFd[2];
    int terr2PipeToChildFd[2];
    int terr1PipeToParentFd[2];
    int terr2PipeToParentFd[2];

    pid_t terr1Pid;
    pid_t terr2Pid;
    char buffer[BUFFER_SIZE];

    //creating pipes
    if( pipe(terr1PipeToChildFd) == -1 || pipe(terr2PipeToChildFd) == -1
        || pipe(terr1PipeToParentFd) == -1 || pipe(terr2PipeToParentFd) == -1){
        fprintf(stderr, "Error while opening pipe.\n");
        return;
    }

    //creating child processes
    terr1Pid = fork();
    if(terr1Pid == -1){
        fprintf(stderr, "Error while creating territory 1 child process.\n");
        return;
    }
    if(terr1Pid > 0){ //original parent process
        //creating territory 2 process
        terr2Pid = fork();
        if(terr2Pid == -1){
            fprintf(stderr, "Error while creating territory 1 child process.\n");
            return;
        }

        if(terr2Pid > 0){ //still the original parent process-------------------------------
            close(terr1PipeToChildFd[0]); //closing child reads
            close(terr2PipeToChildFd[0]);
            close(terr1PipeToParentFd[1]);
            close(terr2PipeToParentFd[1]);
            
            //reading the data
            fseek(fp, 0, SEEK_SET); //set the cursor to the beginning of the file
            while( fgets(buffer, BUFFER_SIZE, fp) != NULL ){ //reading all the lines
                //get the second part of the line - the territory
                char original[BUFFER_SIZE];
                strcpy(original, buffer);
                char * nameCur;
                char * territoryCur;
                nameCur = strtok(buffer, "|");
                territoryCur = strtok(NULL, "|");   
                if(nameCur == NULL || territoryCur == NULL){
                    fprintf(stderr, "Wrong format in data file!");
                    continue;
                }         

                int terrId = isValidTerritory(territoryCur);
                strcat(nameCur, "|");
                if( terrId == 1 ){
                    //fprintf(stdout, "Parent writing to child 1: %s\n", nameCur);
                    write(terr1PipeToChildFd[1], nameCur, strlen(nameCur));
                    //should be separated by string \0...?
                }
                if( terrId == 2 ){
                    //fprintf(stdout, "Parent writing to child 2: %s\n", nameCur);
                    write(terr2PipeToChildFd[1], nameCur, strlen(nameCur));
                }
            }
            write(terr1PipeToChildFd[1], "\0", 1);
            write(terr2PipeToChildFd[1], "\0", 1);

            //waiting for child processes to finish
            int child1Status, child2Status;
            waitpid(terr1Pid,&child1Status,0); 
            waitpid(terr2Pid,&child2Status,0); 
            //fprintf(stdout, "Parent says child processes finished with status %d and %d.\n", child1Status, child2Status);


            char maxName[BUFFER_SIZE] = "No one participated."; //default case
            int maxEggs = 0;
            char * curParticipant;
            char * curParticipantSavePtr; //used for strtok reentry
            char * curName;
            int curEggs;
            char * curNameSavePtr;
            char pipeInput[BUFFER_SIZE];
            int nread = 0;

            fprintf(stdout, "\nReading territory 1 results...\n");
            nread = read(terr1PipeToParentFd[0], pipeInput, BUFFER_SIZE);
            //fprintf(stdout, "parent read: %s, with len %d, and size %d.\n", pipeInput, strlen(pipeInput), nread);

            //fprintf(stdout, "Starting tokenization...\n");
            curParticipant = strtok_r(pipeInput, "|", &curParticipantSavePtr);
            while(curParticipant != NULL){
                //fprintf(stdout, "Next participant: %s\n", curParticipant);
                
                curName = strtok(curParticipant, ";");
                curEggs = atoi( strtok(NULL, ";") );
                fprintf(stdout, "\tparticipant name: %s, eggs: %d\n", curName, curEggs);

                if(curEggs > maxEggs){
                    maxEggs = curEggs;
                    strcpy(maxName, curName);
                }

                curParticipant = strtok_r(NULL, "|", &curParticipantSavePtr);
            }


            fprintf(stdout, "\nReading territory 2 results...\n");
            nread = read(terr2PipeToParentFd[0], pipeInput, BUFFER_SIZE);
            //fprintf(stdout, "parent read: %s, with len %d, and size %d.\n", pipeInput, strlen(pipeInput), nread);

            //fprintf(stdout, "Starting tokenization...\n");
            curParticipant = strtok_r(pipeInput, "|", &curParticipantSavePtr);
            while(curParticipant != NULL){
                //fprintf(stdout, "Next participant: %s\n", curParticipant);
                
                curName = strtok(curParticipant, ";");
                curEggs = atoi( strtok(NULL, ";") );
                fprintf(stdout, "\tparticipant name: %s, eggs: %d\n", curName, curEggs);

                if(curEggs > maxEggs){
                    maxEggs = curEggs;
                    strcpy(maxName, curName);
                }

                curParticipant = strtok_r(NULL, "|", &curParticipantSavePtr);
            }

            fprintf(stdout, "\nThe winner of the competition is: %s, with %d eggs!\n", maxName, maxEggs);

            close(terr1PipeToChildFd[1]); //closing child reads
            close(terr2PipeToChildFd[1]);
            close(terr1PipeToParentFd[0]);
            close(terr2PipeToParentFd[0]);
        }

        if(terr2Pid == 0){ //territory 2 child process-------------------------------------
            close(terr1PipeToChildFd[0]); //closing other territorry pipes
            close(terr1PipeToChildFd[1]);
            close(terr1PipeToParentFd[0]);
            close(terr1PipeToParentFd[1]);

            close(terr2PipeToChildFd[1]); //closing other ends
            close(terr2PipeToParentFd[0]);

            srand(time(NULL)* getpid());
            
            sleep(1); //waiting for parent to write into pipe

            raceManageTerritory(terr2PipeToParentFd[1], terr2PipeToChildFd[0]);

            //fprintf(stdout, "child 2 closing.\n");
            exit(0); //closing child process
        }
    }

    if(terr1Pid == 0){ //territory 1 child process------------------------------------------
        close(terr2PipeToChildFd[0]); //closing other territorry pipes
        close(terr2PipeToChildFd[1]);
        close(terr2PipeToParentFd[0]);
        close(terr2PipeToParentFd[1]);

        close(terr1PipeToChildFd[1]); //closing other ends
        close(terr1PipeToParentFd[0]);

        srand(time(NULL)* getpid());
            
        sleep(1); //waiting for parent to write into pipe

        raceManageTerritory(terr1PipeToParentFd[1], terr1PipeToChildFd[0]);

        //fprintf(stdout, "child 1 closing.\n");
        exit(0); //closing child process
    }
}

void raceManageTerritory(int writeFd, int readFd){

    char pipeInput[BUFFER_SIZE];
    int nread = 0;
    //fprintf(stdout, "terr 2 started reading from pipe...\n");
    nread = read(readFd, pipeInput, BUFFER_SIZE);
    //fprintf(stdout, "terr 2 read: %s, with len %d, and size %d.\n", pipeInput, strlen(pipeInput), nread);

    //fprintf(stdout, "terr 2 finished reading from pipe, starting tokenization...\n");
    char * curName;
    char toWrite[BUFFER_SIZE];
    int curEggs;
    curName = strtok(pipeInput, "|");
    while(curName != NULL){
        //fprintf(stdout, "\tNext name: %s\n", curName);
        
        curEggs = rand() % 100 + 1;
        sprintf(toWrite, "%s;%d|", curName, curEggs);
        //fprintf(stdout, "Writing to parent: %s\n", toWrite);

        write(writeFd, toWrite, strlen(toWrite));

        curName = strtok(NULL, "|");
    }
    write(writeFd, "\0", 1);

    close(readFd);
    close(writeFd);
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

//return number bigger than 0 if terr is in the list of valid territories
// more specifically, returns 1 or 2 based on which race territory it belongs to
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
            contains = ( i < RACE_TERRITORIES_2_START_IND ) ? 1 : 2;
        }
    }

    return contains;
}

void exitProgram(){
    fclose(fp);
    exit(0);
}