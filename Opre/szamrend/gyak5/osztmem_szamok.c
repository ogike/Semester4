#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

struct Numbers
{
    int nums[10];
    int sum;
};

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int pid;
    key_t kulcs;
    int oszt_mem_id;
    struct Numbers *s;

    // a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs = ftok(argv[0], 1);

    // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel, új készítése r+w joggal
    oszt_mem_id = shmget(kulcs, sizeof(struct Numbers), IPC_CREAT | S_IRUSR | S_IWUSR);

    // csatlakozunk az osztott memoriahoz, (shared memory attach)
    s = shmat(oszt_mem_id, NULL, 0);

    pid = fork();
    if (pid > 0)
    {
        for (size_t i = 0; i < 10; i++)
        {
            s->nums[i] = rand() % 10;
            printf("\tA szulo ezt irta az osztott memoriaba: %d\n", s->nums[i]);
        }

        sleep(2); // megvárjuk a sumolást

        printf("A szulo ezt a sumot olvasta ki az osztott memoriabol: %d\n", s->sum);

        // elengedjuk az osztott memoriat (detach)
        shmdt(s);

        wait(NULL);

        // shared memory control (lehet mást is)
        //  IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter (remove id)
        //  IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
        shmctl(oszt_mem_id, IPC_RMID, NULL);
    }
    else if (pid == 0)
    {
        sleep(1);

        s->sum = 0;
        for (size_t i = 0; i < 10; i++)
        {
            s->sum += s->nums[i];
        }

        printf("A gyerek ezt sumolta ki az osztott memoriabol: %d\n", s->sum);

        // gyerek is elengedi
        shmdt(s);
    }

    return 0;
}
