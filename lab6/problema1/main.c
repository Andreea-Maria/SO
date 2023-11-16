#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#define BUFF_SIZE 5

void count_statistics(const char *input_file, const char *output_file, char ch){
int in=open(input_file,O_RDONLY);

    if(in==-1){
        perror("Eroare la deschiderea fisierului de intrare.");
    }

    struct stat file_info;
    //fstat(in, &file_info);
    fstat(in, &file_info);
    off_t file_size=file_info.st_size;

    int lowercase_count=0;
    int uppercase_count=0;
    int digit_count=0;
    int ch_count=0;

    //buffer pt citirea datelor din fisier
    char buff[BUFF_SIZE];
    ssize_t bytes_read;

    //Parcurge fisierul
    while((bytes_read=read(in,buff,sizeof(buff)))>0){
        for(ssize_t i=0;i<bytes_read;++i){
            char current_char =buff[i];

            if(current_char>='a' && current_char<='z'){
                lowercase_count++;
            }
            else if(current_char>='A' && current_char<='Z'){
                uppercase_count++;
            }
            else if(current_char>='0' && current_char<='0'){
                digit_count++;
            }

            if(current_char==ch)
                ch_count++;
        }
    }

    close(in);

    //creeaza fisierul pt output
    char output_string[256];
    sprintf(output_string,"numar litere mici: %d\nnumar litere mari: %d\nnumarcifre: %d\nnumar aparitii caracter: %d\ndimensiune fisier: %ld\n",lowercase_count,uppercase_count,digit_count,ch_count,(long)file_size);

    int out = open(output_file,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(out==-1){
       perror("Eroare la deschiderea fisierului de statistica.");
       close(input_file);
    }

    write(out, output_string,strlen(output_string));
    close(out);
}

int main(int arg, char *args[])
{
    if(arg!=4){
        return 1;
    }

    char *input_file=args[1];
    char *output_file=args[2];
    char ch = args[3][0];
    //int stat(const char *input_filename,struct stat *buf);

    count_statistics(input_file,output_file,ch);


//terminat pb
//st_mode(stat)
//citit directoare
//testat simlink si hardlink



    return 0;
}
