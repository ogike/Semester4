// NOTE: -lrt kapcsoló fordításhoz kell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <errno.h>
// Posix Message queue sample
// mq_open, mq_send,mq_receive, mq_unlink

int main(int argc, char *argv[])
{
	pid_t child;
	int uzenetsor, status;
	char *mqname = "/HM37UQ"; //ez már nem egy rendes elérési út, inkább olyan mint egy aloldal weblapon (topic)
	mqd_t mq1, mq2;

	//mq paraméterei
	struct mq_attr attr;
	attr.mq_maxmsg = 5;	  // MAXMSGS
	attr.mq_msgsize = 50; // MSGSIZE

	//mint a msgget
	mq1 = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
	if (mq1 < 0)
	{
		printf("mq_open error: %d \n", errno);
		return 1;
	}
	char buffer[100]; // mq_send, mq_receive uses char array
	printf("Buffer length: %d\n", strlen(buffer));
	child = fork();
	
    // mq2=mq_open(mqname,O_RDWR);
    // strcpy(buffer,"                  ");

    // 5 is the message priority
    //  itt nem kell explicit megadni, hogy melyik akarjuk megadni
    //		de ha akarjuk a NULL helyébe megadhatjuk explicit
    //  tehát a prioritás az tényleg prioritás, nem úgy mint az előző msgQueue-mél
    int db = mq_receive(mq1, buffer, 54, NULL);
    //
    printf("Buffer length: %d\n", strlen(buffer));
    printf("mq_receive : %s, olvasott bajtok hossza: %d errno: %s\n", buffer, db, strerror(errno));

    mq_close(mq1);

    // mq_send(mq1,"Gyerek befejezte!", 20,6); //kétirányú kommunikáció is akár
    // msgsnd(uzenetsor,&valasz,sizeof(struct uzenet),0);
    return 0;

}
