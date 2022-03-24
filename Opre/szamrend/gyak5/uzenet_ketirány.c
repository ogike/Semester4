#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

//fontos hogy legyen mtype mezője!
struct uzenet_str { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
                    // háttérben a msg queu fel van bontva ezekre a típusokra (mintha alcsövek lennek
                    // ezen belül melyik csövön kükdje 
     char mtext [ 1024 ]; //pointereket nem lehet küldeni - ezért az egész karakter tömböt át kell küldeni, meg kell mondani mennyi a max mérete
}; 

struct uzenet_nums {
    long mtype;
    int mnums[5];
}

// sendig a message
int kuld( int uzenetsor, int isNums ) 
{ 
    int status;
    if(isNums == 0){
        int[] nums = {0,2,4,6,8};
        const struct uzenet_nums uz = { 4, {0,2,4,6,8} }; 
        status = msgsnd uzenetsor, &uz, sizeof(struct uzenet_nums) - sizeof(long) , 0 ); 
    } else{
        int[] nums = {0,2,4,6,8};
        const struct uzenet_str uz = { 5, "Hajra Fradi!" }; 
        status = msgsnd uzenetsor, &uz, sizeof(struct uzenet_str) - sizeof(long) , 0 ); 
    }
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad_str( int uzenetsor ) 
{ 
     struct uzenet_str uz; 
     int status; 

     // az utolso parameter(0) az uzenet azonositoszama
	//   ha az 0, akkor a sor elso uzenetet vesszuk ki
	//   ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

// receiving a message. 
int fogad_nums( int uzenetsor ) 
{ 
     struct uzenet_nums uz; 
     int status; 

     // az utolso parameter(0) az uzenet azonositoszama
	//   ha az 0, akkor a sor elso uzenetet vesszuk ki
	//   ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %d %d %d %d %d\n", 
          uz.mtype, uz.mnums[0], uz.mnums[1], uz.mnums[2], uz.mnums[3], uz.mnums[4]); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok general hasheléssel  
     kulcs = ftok(argv[0],1); //ftok: kér egy elérési utat
     printf ("A kulcs: %d\n",kulcs);

     //msgget kulcs: egyedi azon opre. szinten
     // második param: milyen jogosultág | milyen file módba nyissa meg kb
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
          kuld( uzenetsor, 1 );  // Parent sends a message. 
          wait( 0 ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) {
         kuld( uzenetsor, 0); 
          return fogad( uzenetsor ); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
