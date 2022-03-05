#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//ezzel a SIGTERM utasítás alap funkcionalitását felülírtam
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

  //signal: egy jel, ami akár minden mást megszakít

  //ezzel feliratkozzunk erre a signalra -> hanlder kezelje le
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();
  if (child>0) //parent
  { 
    pause(); //waits till a signal arrive 
    printf("Signal arrived\n",SIGTERM);
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else  //child
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    sleep(3);

    //NOTE: függetlenül névtúl, nem kell megállnia a kill parancsra
    kill(getppid(),SIGTERM); //sigterm-et kiküldi
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}
