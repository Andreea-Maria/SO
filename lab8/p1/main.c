#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <errno.h>


int main(char argc, char *argv[])
{
    //const char *filename="studenti.c";
    fork();

    int pid=getpid();
    int ppid=getppid();

    if(pid<0){
        perror("EROARE PROCES COPIL.");
        exit(EXIT_FAILURE);
    }
    if(pid==0){
       // execlp("ls","ls","-l",NULL);
    }



    return 0;
}
