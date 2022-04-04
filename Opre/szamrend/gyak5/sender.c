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
	char *mqname = "/jo3em3mq"; //ez már nem egy rendes elérési út, inkább olyan mint egy aloldal weblapon (topic)
	// char *mqname = "/HM37UQ"; //ez már nem egy rendes elérési út, inkább olyan mint egy aloldal weblapon (topic)
	mqd_t mq1, mq2;

	//mq paraméterei
	struct mq_attr attr;
	attr.mq_maxmsg = 5;	  // MAXMSGS
	attr.mq_msgsize = 50; // MSGSIZE

	//mint a msgget
	mq1 = mq_open(mqname, O_RDWR, 0666, &attr);
	if (mq1 < 0)
	{
		printf("mq_open error: %d \n", errno);
		return 1;
	}

	char buffer[100]; // mq_send, mq_receive uses char array
	printf("Buffer length: %d\n", strlen(buffer));
    char uzenet[30] = "Hello HM37UQ";

    // uzenet = stdin()

    uzenet[29] = 65;

    int db = mq_send(mq1, uzenet, strlen(uzenet), 5); // Parent sends a message.
    // 5 priority, higher number, higher priority

    // After terminating child process, the message queue is deleted.
    mq_close(mq1); //az én kontextusomban (folyamatomban) lévő mq rész
    printf("Szulo vege!\n\n");

	return 0;
}
