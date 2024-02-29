#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){
 int par; 
 int chi; 
 pid_t  pid;
 int status;

 if(argc!=3)
 {
 	 printf("Aufruf: block x y mit x=Parent-Block-Zeit und y=Child-Block-Zeit\n");
	 return 1;
 }
 pid=fork();
 if(pid<0)
 {
 	 printf("Fehler: fork()-Rueckgabe %d.\n", pid);
	 exit(2);
 }
 if(pid==0)
 { //Child
 	 chi=atoi(argv[2]);
 	 printf("\t\t\t\t\t\tChild: PID=%i PPID=%i Blocked for %is",getpid(),getppid(),chi);
 	 fflush(stdout);
 	 sleep(chi);
 	 exit(3);
 }else
 { // Parent, pid=PID des Childs
  	p=atoi(argv[1]);
  	printf("Parent: PID=%i PPID=%i Blocked for %is\r",getpid(),getppid(),par);
  	fflush(stdout);
  	sleep(par);
  	pid=wait(&status);
  	printf("\nExit-Status des Child: ");
  	if(WIFEXITED(status)!=0)
        printf("%d\n",WEXITSTATUS(status));
  	else printf("Fehler\n");
 }
 return 0;
}
