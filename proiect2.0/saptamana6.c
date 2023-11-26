#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#define st_mtime st_mtim.tv_sec
#define BUF_SIZE 5


int main(char argc, char *argv[]){
    if(argc!=2){
        perror("Usage ./program <fisier_intrare");
        exit(EXIT_FAILURE);
    }

    char *input_filename=args[1];
    char output_filename[]="statistica.txt";
    struct stat file_stat;

    if(stat(input_filename,O_RDONLY)==-1){
        perror("Eroare deschidere fisier de intrare.");
    }

    char buff[BUF_SIZE];
    ssize_t bytes_read;
    char output_string[256];
    int in = open(input_filename, O_RDONLY);

    read(in,buff,sizeof(buff));
    fstat(in,&file_stat);
    off_t dimensiune = file_stat.st_size;
    uid_t IDuser = file_stat.st_uid;
    struct timespec timpulUltimeiModificari = file_stat.st_mtim;
    nlink_t numarLegaturi = file_stat.st_nlink;
    char drepturiUser[4];
    (file_stat.st_mode & S_IRUSR) ? strcpy(drepturiUser,'R') : strcpy(drepturiUser,'-');
    (file_stat.st_mode & S_IWUSR) ? strcpy(drepturiUser,'W') : strcpy(drepturiUser,'-');
    (file_stat.st_mode & S_IXUSR? strcpy(drepturiUser,'X') : strcpy(drepturiUser,'-');
    char *drepturiGrup;
    (file_stat & S_ISRGRP)? strcpy(drepturiGrup,'R') : strcpy(drepturiGrup,'-');
    (file_stat & S_ISWGRP) ? strcpy(drepturiGrup,'W') : strcpy(drepturiGrup,'-');
    (file_stat & S_ISXGRP) ? strcpy(drepturiGrup,'X') : strcpy(drepturiGrup,'-');
    char drepturiAltii[4];
    (file_stat & S_IROTH) ? strcpy(drepturiAltii,'R') : strcpy(drepturiAltii,'-');
    (file_stat & S_IWOTH) ? strcpy(drepturiAltii,'W') : strcpy(drepturiAltii,'-');
    (file_stat & S_IXOTH) ? strcpy(drepturiAltii,'X') : strcpy(drepturiAltii,'-');
    sprntf(output_string, "nume fisier: poza.bmp\n dimensiune: %d\n identificatorul utilizatorului :%s\n timpul ultimei modificari: %d\n contorul de legaturi : %s\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii: %s",dimensiune,IDuser,timpulUltimeiModificari,numarLegaturi,drepturiUser,drepturiGrup,drepturiAltii);
    
    close(in);

    //crearea fisierului
    int out = open(output_filename,O_CREAT | O_RDONLY | O_WRONLY | O_TRUNC,S_IRUSR | S_IWUSR | S_IXUSR);
    if(out==-1){
        perror("Eroare creare fisier statistica.");
        exit(EXIT_FAILURE);
    }
    write(out,*buff,strlen(output_string));
    close(out);

}
