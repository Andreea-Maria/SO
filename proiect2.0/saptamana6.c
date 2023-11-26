#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define st_mtime st_mtim.tv_sec
#define BUF_SIZE 5

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Usage ./program <fisier_intrare");
        exit(EXIT_FAILURE);
    }

    char *input_filename = argv[1];
    char output_filename[] = "statistica.txt";
    struct stat file_stat;

    if (stat(input_filename, &file_stat) == -1) {
        perror("Eroare deschidere fisier de intrare.");
        exit(EXIT_FAILURE);
    }

    char buff[BUF_SIZE];
    ssize_t bytes_read;
    char output_string[256];
    int in = open(input_filename, O_RDONLY);

    read(in, buff, sizeof(buff));
    fstat(in, &file_stat);
    off_t dimensiune = file_stat.st_size;
    uid_t IDuser = file_stat.st_uid;
    struct timespec timpulUltimeiModificari = file_stat.st_mtim;
    nlink_t numarLegaturi = file_stat.st_nlink;
    char drepturiUser[4];
    (file_stat.st_mode & S_IRUSR) ? (drepturiUser[0] = 'R') : (drepturiUser[0] = '-');
    (file_stat.st_mode & S_IWUSR) ? (drepturiUser[1] = 'W') : (drepturiUser[1] = '-');
    (file_stat.st_mode & S_IXUSR) ? (drepturiUser[2] = 'X') : (drepturiUser[2] = '-');
    drepturiUser[3] = '\0';
    char drepturiGrup[4];
    (file_stat.st_mode & S_IRGRP) ? (drepturiGrup[0] = 'R') : (drepturiGrup[0] = '-');
    (file_stat.st_mode & S_IWGRP) ? (drepturiGrup[1] = 'W') : (drepturiGrup[1] = '-');
    (file_stat.st_mode & S_IXGRP) ? (drepturiGrup[2] = 'X') : (drepturiGrup[2] = '-');
    drepturiGrup[3] = '\0';
    char drepturiAltii[4];
    (file_stat.st_mode & S_IROTH) ? (drepturiAltii[0] = 'R') : (drepturiAltii[0] = '-');
    (file_stat.st_mode & S_IWOTH) ? (drepturiAltii[1] = 'W') : (drepturiAltii[1] = '-');
    (file_stat.st_mode & S_IXOTH) ? (drepturiAltii[2] = 'X') : (drepturiAltii[2] = '-');
    drepturiAltii[3] = '\0';

    sprintf(output_string, "nume fisier: poza.bmp\n dimensiune: %ld\n identificatorul utilizatorului :%u\n timpul ultimei modificari: %02ld \n contorul de legaturi : %u\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii: %s", dimensiune, IDuser, (long)timpulUltimeiModificari.tv_sec, numarLegaturi, drepturiUser, drepturiGrup, drepturiAltii);

    close(in);

    // Crearea fisierului
    int out = open(output_filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
    if (out == -1) {
        perror("Eroare creare fisier statistica.");
        exit(EXIT_FAILURE);
    }
    write(out, output_string, strlen(output_string));
    close(out);

    return 0;
}
