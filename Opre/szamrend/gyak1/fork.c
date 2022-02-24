#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 int notacommonvalue=1;
 printf("The value is %i before forking \n",notacommonvalue);
 
 //csinál egy copy-t a folyamatról  (és visszaadja a process azonosítóval)
   //ez a process azonosító pozitív (tehát ha minusz akkor error)
   //ha child, akkor 0 
 pid_t  child=fork(); //forks make a copy of variables
 pid_t child2=fork(); //ez a child1-ben is forkolódik (összesen így lesz 4 process)
                      //ha akarnám hogy csak 3 legyen, akkor a child > 0 ágban kell
                        //zh-ban lehet rámennek erre!
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    waitpid(child,&status,0); 
    //waits the end of child process PID number=child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)
    printf("The value is %i in parent process (remain the original) \n",notacommonvalue);
 
 }
 else //child process
 {
    notacommonvalue=5; //it changes the value of the copy of the variable
    printf("The value is %i in child process \n",notacommonvalue);
 }
 return 0;
}