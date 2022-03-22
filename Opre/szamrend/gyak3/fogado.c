#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>



// a fogado felelőssége lesz a pipe
int main()
{
    int pid, fd;
    printf("Fifo start!\n");
    char pipename[20];

    sprintf(pipename, "/tmp/HM37UQ_nevcso");
    int fid = mkfifo(pipename, S_IRUSR | S_IWUSR); // creating named pipe file
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
    printf("Csonyitas eredmenye szuloben: %d!\n", fid);
    fd = open(pipename, O_RDONLY);

    int number = -1;
    while(number != 0)
    {
        read(fd,  &number, sizeof(int));
        printf("Ezt olvastam a csobol: %d \n", number);
    }
    printf("0-t olvastam, program vége.")

    close(fd);
    unlink(pipename); // remove fifo.ftc
    return 0;
}