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
    int result = scanf("%d", &number);

    if (result == 0)
    {
        printf("Sikertelen olvasas\n");
    }
    else
    {
        printf("Sikeres iras\n");
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

    //neki getpid..?
    sprintf(pipename, "/tmp/HM37UQ_nevcso", getpid());

    //neki nincs mkfifo!!!
    //int fid = mkfifo(pipename, S_IRUSR | S_IWUSR); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask

    int number = -1;
    fd = open(pipename, O_WRONLY);
    while(number != 0)
    {
        number = int_read();
        write(fd, &number, sizeof(int));
        printf("Gyerek vagyok, beirtam a következő számot!: %d\n", number);
    }
    printf("Mkfifo vege!\n");

    close(fd);
    return 0;
}