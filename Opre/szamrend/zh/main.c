#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>


#define MSGSIZE 50
#define Hazaterhet SIGTERM
#define Keress_meg SIGUSR1

void handler(int signumber){
    printf("Signal with number %i has arrived\n",signumber);
}

void hazater_handler(int signumber){
    printf("Nyuszi hazatért.\n",signumber);
    exit(0);
}


int main(){

    //üzenetsor
    int uzenetsor, status;
	char *mq1name = "/HM37UQ";
	char *mq2name = "/HM37UQ";
	mqd_t mq1, mq2;

	//mq paraméterei
	struct mq_attr attr;
	attr.mq_maxmsg = 5;	  // MAXMSGS
	attr.mq_msgsize = MSGSIZE; // MSGSIZE

	//tapsi üzenetsora
	mq_unlink(mq1name); // delete if exist
	mq1 = mq_open(mq1name, O_CREAT | O_RDWR, 0600, &attr);
	if (mq1 < 0)
	{
		printf("mq_open error for tapsi: %d \n", errno);
		return 1;
	}

	//füles üzenetsora
	mq_unlink(mq2name); // delete if exist
	mq2 = mq_open(mq2name, O_CREAT | O_RDWR, 0600, &attr);
	if (mq1 < 0)
	{
		printf("mq_open error for hapsi: %d \n", errno);
		return 1;
	}
    
    //forking
    pid_t terr1Pid; //tapsi
    pid_t terr2Pid; //füles

    terr1Pid = fork();
    if(terr1Pid == -1){
        fprintf(stderr, "Error while creating territory 1 child process.\n");
        return 1;
    }

    if(terr1Pid > 0){ //original parent process
        //creating territory 2 process
        terr2Pid = fork();
        if(terr2Pid == -1){
            fprintf(stderr, "Error while creating territory 1 child process.\n");
            return 1;
        }

        if(terr2Pid > 0){ //still the original parent process-------------------------------
            signal(SIGUSR1, handler);
            signal(SIGUSR2, handler);

            pause(); //waits till first signal
            pause(); //waits till second signal
            
            fprintf(stdout, "Gyerekek felkészültek, írás kezdés!\n\n");
            
            char terr1[MSGSIZE] = "Lovas";
            char terr2[MSGSIZE] = "Szula";

            int db1 = mq_send(mq1, terr1, strlen(terr1)+1, 5);
            int db2 = mq_send(mq2, terr2, strlen(terr2)+1, 5);


            sleep(1);
            fprintf(stdout, "Szülő vár gyerekeinek számlálására...\n\n");
            sleep(1);

            char buffer[MSGSIZE];
            int bufferInt;
            srand(time(NULL)* getpid());

            fprintf(stdout, "Tapsitól olvas szülő...\n");
            mq_receive(mq1, buffer, MSGSIZE, NULL);
            bufferInt = atoi(buffer);
            int tapsiPrev = rand() % 50 + 50;
		    fprintf(stdout, "Tapsi nyusziszáma : %s, előző nyusziszám: %d\n", buffer, tapsiPrev);
            if(bufferInt > tapsiPrev){ //gyarapodás
                fprintf(stdout, "\tVáltozás: gyarapodás\n");
            } else{
                fprintf(stdout, "\tVáltozás: nyuszifogyás\n");
            }

            fprintf(stdout, "Fülestől olvas szülő...\n");
            mq_receive(mq2, buffer, MSGSIZE, NULL);
            bufferInt = atoi(buffer);
            int fulesPrev = rand() % 50 + 50;
            fprintf(stdout, "Füles nyusziszáma : %s, előző nyusziszám: %d\n", buffer, fulesPrev);
            if(bufferInt > fulesPrev){ //gyarapodás
                fprintf(stdout, "\tVáltozás: gyarapodás");
            } else{
                fprintf(stdout, "\tVáltozás: nyuszifogyás");
            }


            //waiting for child processes to finish
            int child1Status, child2Status;
            waitpid(terr1Pid,&child1Status,0); 
            waitpid(terr2Pid,&child2Status,0); 
            fprintf(stdout, "\nSzülő: Tapsi és füles is nyugovóra térte, státuszuk: %d and %d.\n", child1Status, child2Status);

            mq_close(mq1);
            mq_close(mq2);
            mq_unlink(mq1name);
            mq_unlink(mq2name);
        }

        if(terr2Pid == 0){ //territory 2 child process-------------------------------------
            fprintf(stdout, "Fules felkeszules...\n");
            sleep(1);
            kill(getppid(), SIGUSR2);
            fprintf(stdout, "Fules felkeszult.\n");

            char terrNameBuffer[MSGSIZE];
            sleep(1);
            fprintf(stdout, "Füles olvas...\n");
            int db = mq_receive(mq2, terrNameBuffer, MSGSIZE, NULL);
		    printf("Füles területe : %s\n", terrNameBuffer);

            srand(time(NULL)* getpid());
            int nyusziCount = rand() % 50 + 50;
            char msgToParent[MSGSIZE];
            sprintf(msgToParent, "%d", nyusziCount);
            printf("Füles sending to parent: %s\n", msgToParent);
            int db2 = mq_send(mq2, msgToParent, strlen(msgToParent)+1, 3);

            mq_close(mq1);
            mq_close(mq2);
            exit(0); //closing child process
        }
    }

    if(terr1Pid == 0){ //territory 1 child process------------------------------------------   
        fprintf(stdout, "Tapsi felkeszules...\n");
        sleep(1);
        kill(getppid(), SIGUSR1);
        fprintf(stdout, "Tapsi felkeszult.\n");

            char terrNameBuffer[MSGSIZE];
        sleep(1);
        fprintf(stdout, "Tapsi olvas...\n");
        int db = mq_receive(mq1, terrNameBuffer, MSGSIZE, NULL);
		printf("Tapsi területe:  %s\n", terrNameBuffer);

        srand(time(NULL)* getpid());
        int nyusziCount = rand() % 50 + 50;
        char msgToParent[MSGSIZE];
        sprintf(msgToParent, "%d", nyusziCount);
        printf("Tapsi sending to parent: %s\n", msgToParent);
        int db1 = mq_send(mq1, msgToParent, strlen(msgToParent)+1, 3);

        mq_close(mq1);
        mq_close(mq2);
        exit(0); //closing child process
    }

    return 0;
}