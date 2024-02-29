#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
 	char befehl[]="./flip";
 	int z;
 	char b2[10];
 	if(argc!=2)
 	{
 		 printf("Aufruf: flip x mit x=Anzahl der Aufrufe\n");
 		 return 1;
 	}
 	z=atoi(argv[1]);
 	printf("\t\t\t\t\t\t\tflop | argc=%i | argv[1]=%i | ",argc,z);
 	printf("PID %i | PPID %i \r",getpid(),getppid());
 	fflush(stdout);
 	sleep(1);
 	if(z>0)
 	{
 		 sprintf(b2,"%i",--z);
 		 execl(befehl,befehl,b2,0);
 	}
 	else printf("\nFlop Ende\n");
 	return 0;
}
