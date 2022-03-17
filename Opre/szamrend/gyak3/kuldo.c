#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // for errno, the number of last error

int int_read()
{
    int number = -1;

    printf("Kérek egy számot!: ");
    int result = scanf(&number);

    if (result == 0)
    {
        printf("Sikertelen");
    }
    else
    {
        printf("Sikeres");
    }

    // sorvégi karakterek eldobása
    while (fgetc(stdin) != '\n')
        ;

    return number;
}

int main()
{
    int pid, fd;
    printf("Fifo start!\n");
    char pipename[20];

    sprintf(pipename, "/tmp/HM37UQ_nevcso");

    //neki nincs mkfifo!!!
    //int fid = mkfifo(pipename, S_IRUSR | S_IWUSR); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask

    if (fid == -1)
    {
        printf("Error number: %i", errno);
        perror("Gaz van:");
        exit(EXIT_FAILURE);
    }
    printf("Mkfifo vege!\n");


    printf("Gyerek vagyok, irok a csobe!\n");
    printf("Csonyitas eredmenye: %d!\n", fid);
    fd = open(pipename, O_WRONLY);
    write(fd, "Hajra Fradi!\n", 12);


    close(fd);
    printf("Gyerek vagyok, beirtam, vegeztem!\n");
}