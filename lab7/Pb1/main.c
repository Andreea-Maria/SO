#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(int argc, char argv[]){

    if(argc!=2)
        perror("Argumentele date nu sunt conform cerintei.");
    //deschiderea fisierului pentru citire
    int fd = open(arg[1],O_RDONLY);

    if(fd==-1){
        perror("Eroare la deschiderea fisierului de intrare.");
    }

    struct stat file_stat;
    char *buff;
    size_t nbytes;

    if(stat(fd, &file_stat)==-1){
        perror("Eroare accesare file_stat.");
    }

    while (nbytes= read(fd,*buff,sizeof(buff))>0){

    }
}
