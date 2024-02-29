#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>



void writeDataToFile(int num)
{
    FILE *file =fopen("daten.txt","w");
    if(file == NULL)
    {
        perror("Failed to open file");
        exit(1);
    }

    pid_t pid =getpid();

    for(int i=1; i<= num; i++)
    {
        fprintf(file,"< Writer-PID: %d -> record-nr: %d >\n",pid,i);
        sleep(1);
    }

    fclose(file);
}

void readDataFromFile()
{
    FILE *file =fopen("daten.txt","r");
    if(file == NULL)
    {
        perror("Failed to open file");
        exit(1);
    }

    char buffer[256];
    pid_t pid=getpid();
    int nr=1;

    
    while(fgets(buffer,sizeof(buffer),file) != 0)
    {
        printf("< Reader[A]-PID: %d -> record-nr: %d > -> %s\n",pid,nr,buffer);
        
        nr=nr+1;
    }
    fclose(file);
    
}


int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        printf("Usage: ./reader <numEntries>\n");
        return 1;
    }


    int numEntries = atoi(argv[1]);
    pid_t pid_child1,pid_child2,pid_child3;
    char buffer[256];
    ssize_t bytes_read;

    int p[2]; //pipe


    if(pipe(p) < 0)
    {
        perror("failed to create Pipe");
        exit(1);
    }



    pid_child1=fork();
    if(pid_child1 < 0)
    {
        perror("Failed to fork\n");
        return 1;
    }
    else if(pid_child1 == 0)
    {
        writeDataToFile(numEntries);
        
        close(p[0]);
        int i=1;
        for(i;i<=numEntries;i++)
        {
            dprintf(p[1],"< Writer-PID: %d -> record-nr: %d >\n",pid_child1,i);
            sleep(1);
        }
        close(p[1]);
        exit(101);
    }

    pid_child2=fork();
    if(pid_child2 < 0)
    {
        perror("Failed to fork");
        return 1;
    }
    else if(pid_child2 == 0)
    {
        close(p[0]);
        close(p[1]);
        readDataFromFile();
        exit(102);
    }


    pid_child3=fork();
    if(pid_child3 < 0)
    {
        perror("Failed to fork");
        return 1;
    }
    else if(pid_child3 == 0)
    {
        close(p[1]);
        int i=1;
        
        while((bytes_read=read(p[0],buffer,sizeof(buffer)-1)) > 0)
        {
            buffer[bytes_read]='\0';
            printf("< Reader[B]-PID: %d -> record-nr: %d > -> %s\n",pid_child3,i,buffer);
            
            i++;
        }

        close(p[0]);
        exit(0);
    }

    close(p[0]);
    close(p[1]);
    
    int status;
    waitpid(pid_child1,&status,-1);
    waitpid(pid_child2,&status,-1);
    waitpid(pid_child3,&status,-1);



    printf("Exit status of Child 1: %d\n", WEXITSTATUS(status));
    printf("Exit status of Child 2: %d\n", WEXITSTATUS(status));
    printf("Exit status of Child 3: %d\n", WEXITSTATUS(status));

    

    return 0;
}
