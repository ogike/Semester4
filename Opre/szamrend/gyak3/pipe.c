#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <sys/wait.h>
//
// unnamed pipe example
//
int main(int argc, char *argv[])
{
    int pipefd[2]; // unnamed pipe file descriptor array
    pid_t pid;
    char sz[100]; // char array for reading from pipe

    // csővezeték létrehozása
    //  paraméter: kettő elemű tömb (kettő file descriptor = szám)
        //fd: minden egyes folyamathoz tartozik egy fd
            //gyakorlatilag file, ami számmal van jelölve
        //két fd: [read, write]
        // => ugyan azt a write()-ot tudjuk használni process-ek közötti kommunkiációra, mint fileoknál
    if (pipe(pipefd) == -1)
    {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    pid = fork(); // creating parent-child processes
    if (pid == -1)
    {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {                     // child process, ami olvas
        sleep(3);         // sleeping a few seconds, not necessary
        close(pipefd[1]); // Usually we close the unused write end
                            //szokás, mivel nem fogunk írni

        int length = -1 ; //TODO: test
        read(pipefd[0], &length, sizeof(int)); // reading 1 char for length
        printf("Gyerek kiolvassa az adat meretet: %d\n", length);
        

        printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
        read(pipefd[0], sz, length); // reading max 100 chars
        printf("Gyerek olvasta uzenet: %s\n", sz);

        printf("Gyerek elkezdi másodszorra olvasni a csobol az adatokat!\n");
        read(pipefd[0], sz, sizeof(sz)); // reading max 100 chars
        printf("Gyerek olvasta uzenet: %s\n", sz);

        printf("\n");
        close(pipefd[0]); // finally we close the used read end
    }
    else
    { // szulo process
        printf("Szulo indul!\n");
        close(pipefd[0]); // Usually we close unused read end
                            //szokás, mivel nem fogunk olvasni

        int msgLength = strlen("Hajra Fradi!") + 1;
        write(pipefd[1], &msgLength, sizeof(int));

        write(pipefd[1], "Hajra Fradi!", msgLength);
        write(pipefd[1], "Hajra Fradi2!", 14); //ez már 14 hosszú lenne, vége nem megy át
        close(pipefd[1]); // Closing write descriptor
        printf("Szulo beirta az adatokat a csobe!\n");
        fflush(NULL); // flushes all write buffers (not necessary)
        wait(NULL);   // waiting for child process (not necessary)
                      // try it without wait()
        printf("Szulo befejezte!\n");
    }
    exit(EXIT_SUCCESS); // force exit, not necessary
}
