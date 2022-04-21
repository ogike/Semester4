#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[])
{

   int pid;
   key_t kulcs;
   int oszt_mem_id;
   char *s;

   // a parancs nevevel es az 1 verzio szammal kulcs generalas
   kulcs = ftok(argv[0], 1);

   // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel, új készítése r+w joggal
   oszt_mem_id = shmget(kulcs, 500, IPC_CREAT | S_IRUSR | S_IWUSR);

   // csatlakozunk az osztott memoriahoz, (shared memory attach)
   // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
   // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van (most nem az)
   s = shmat(oszt_mem_id, NULL, 0);  
   
   pid = fork();
   if (pid > 0)
   {

      char buffer[] = "Hajra Fradi! \n";
      // beirunk a memoriaba
      strcpy(s, buffer);

      // elengedjuk az osztott memoriat
      shmdt(s);
      //	s[0]='B';  //ez segmentation fault hibat ad

      wait(NULL);

      //shared memory control (lehet mást is)
      // IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter (remove id)
      // IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
      //  ami shmid_ds struct tipusu mutato
      // ezt is csak a "main" tudja csinálni
      shmctl(oszt_mem_id, IPC_RMID, NULL);
   }
   else if (pid == 0)
   {
      sleep(1);
      printf("A gyerek ezt olvasta az osztott memoriabol: %s", s);

      // gyerek is elengedi
      shmdt(s);
   }

   return 0;
}
