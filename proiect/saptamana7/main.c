#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dirent.h>
#include <string.h>
#include <librt

void afiseaza_eroare(char *mesaj) {
    perror(mesaj);
    exit(EXIT_FAILURE);
}

void afiseaza_utilizare() {
    fprintf(stderr, "Utilizare: ./program <director_intrare>\n");
    exit(EXIT_FAILURE);
}

void proceseaza_fisier_bmp(char *cale_fisier, int stat_fd) {
    // Deschide fisierul BMP in modul de citire
    int bmp_fd = open(cale_fisier, O_RDONLY);
    if (bmp_fd == -1) {
        afiseaza_eroare("Eroare la deschiderea fisierului BMP");
    }

    // Citeste dimensiunile imaginii BMP din header
    unsigned char header[54]; // Header BMP are 54 de octeti
    if (read(bmp_fd, header, sizeof(header)) != sizeof(header)) {
        afiseaza_eroare("Eroare la citirea header-ului BMP");
    }

    int latime = *(int*)&header[18];
    int inaltime = *(int*)&header[22];

    // Creeaza sirul pentru drepturile de acces
    char drepturi_acces[10];
    sprintf(drepturi_acces, "%c%c%c",
            (header[16] & 4) ? 'R' : '-',
            (header[16] & 2) ? 'W' : '-',
            (header[16] & 1) ? 'X' : '-');

    // Creeaza sirul formatat si il scrie in fisier
    char buffer_statistic[256];
    sprintf(buffer_statistic,
            "nume fisier: %s\n"
            "inaltime: %d\n"
            "latime: %d\n"
            "dimensiune: %ld\n"
            "identificatorul utilizatorului: %d\n"
            "timpul ultimei modificari: %ld\n"
            "contorul de legaturi: %ld\n"
            "drepturi de acces user: %s\n"
            "drepturi de acces grup: %c%c%c\n"
            "drepturi de acces altii: %c%c%c\n",
            cale_fisier, inaltime, latime,
            lseek(bmp_fd, 0, SEEK_END),
            0, 0, 0, // Valori temporare
            drepturi_acces,
            (header[14] & 4) ? 'R' : '-',
            (header[14] & 2) ? 'W' : '-',
            (header[14] & 1) ? 'X' : '-',
            (header[15] & 4) ? 'R' : '-',
            (header[15] & 2) ? 'W' : '-',
            (header[15] & 1) ? 'X' : '-');

    if (write(stat_fd, buffer_statistic, sizeof(buffer_statistic)) == -1) {
        afiseaza_eroare("Eroare la scrierea in fisierul statistic");
    }

    // Inchide fisierul BMP
    if (close(bmp_fd) == -1) {
        afiseaza_eroare("Eroare la inchiderea fisierului BMP");
    }
}

void proceseaza_intrare(char *nume_intrare, char *cale_parinte, int stat_fd) {
    char cale_completa[256];
    sprintf(cale_completa, "%s/%s", cale_parinte, nume_intrare);

    struct stat stat_intrare;
    if (lstat(cale_completa, &stat_intrare) == -1) {
        afiseaza_eroare("Eroare la obtinerea informatiilor despre fisier");
    }

    char buffer_statistic[256];

    if (S_ISREG(stat_intrare.st_mode)) {
        if (strstr(nume_intrare, ".bmp") != NULL) {
            proceseaza_fisier_bmp(cale_completa, stat_fd);
        } else {
            sprintf(buffer_statistic,
                    "nume fisier: %s\n"
                    "dimensiune: %ld\n"
                    "identificatorul utilizatorului: %d\n"
                    "timpul ultimei modificari: %ld\n"
                    "contorul de legaturi: %ld\n"
                    "drepturi de acces user: %s\n"
                    "drepturi de acces grup: %c%c%c\n"
                    "drepturi de acces altii: %c%c%c\n",
                    nume_intrare, (long)stat_intrare.st_size, (int)stat_intrare.st_uid,
                    stat_intrare.st_mtime, (long)stat_intrare.st_nlink,
                    (stat_intrare.st_mode & S_IRUSR) ? 'R' : '-',
                    (stat_intrare.st_mode & S_IWUSR) ? 'W' : '-',
                    (stat_intrare.st_mode & S_IXUSR) ? 'X' : '-',
                    (stat_intrare.st_mode & S_IRGRP) ? 'R' : '-',
                    (stat_intrare.st_mode & S_IWGRP) ? 'W' : '-',
                    (stat_intrare.st_mode & S_IXGRP) ? 'X' : '-',
                    (stat_intrare.st_mode & S_IROTH) ? 'R' : '-',
                    (stat_intrare.st_mode & S_IWOTH) ? 'W' : '-',
                    (stat_intrare.st_mode & S_IXOTH) ? 'X' : '-');
        }
    } else if (S_ISDIR(stat_intrare.st_mode)) {
        sprintf(buffer_statistic,
                "nume director: %s\n"
                "identificatorul utilizatorului: %d\n"
                "drepturi de acces user: %s\n"
                "drepturi de acces grup: %c%c%c\n"
                "drepturi de acces altii: %c%c%c\n",
                nume_intrare, (int)stat_intrare.st_uid,
                (stat_intrare.st_mode & S_IRUSR) ? 'R' : '-',
                (stat_intrare.st_mode & S_IWUSR) ? 'W' : '-',
                (stat_intrare.st_mode & S_IXUSR) ? 'X' : '-',
                (stat_intrare.st_mode & S_IRGRP) ? 'R' : '-',
                (stat_intrare.st_mode & S_IWGRP) ? 'W' : '-',
                (stat_intrare.st_mode & S_IXGRP) ? 'X' : '-',
                (stat_intrare.st_mode & S_IROTH) ? 'R' : '-',
                (stat_intrare.st_mode & S_IWOTH) ? 'W' : '-',
                (stat_intrare.st_mode & S_IXOTH) ? 'X' : '-');
    } else if (S_ISLNK(stat_intrare.st_mode)) {
        // ... (ca in versiunea anterioara)
    }

    // Scrie in fisierul statistic.txt
    if (write(stat_fd, buffer_statistic, strlen(buffer_statistic)) == -1) {
        afiseaza_eroare("Eroare la scrierea in fisierul statistic");
    }
}

void proceseaza_director(char *cale_director, int stat_fd) {
    DIR *director = opendir(cale_director);
    if (!director) {
        afiseaza_eroare("Eroare la deschiderea directorului");
    }

    struct dirent *intrare;
    while ((intrare = readdir(director)) != NULL) {
        if (strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0) {
            proceseaza_intrare(intrare->d_name, cale_director, stat_fd);
        }
    }

    closedir(director);
}

int main(int argc, char *argv[]) {
    // Verifica numarul corect de argumente
    if (argc != 2) {
        afiseaza_utilizare();
    }

    // Deschide directorul in modul de citire
    int dir_fd = open(argv[1], O_RDONLY);
    if (dir_fd == -1) {
        afiseaza_eroare("Eroare la deschiderea directorului de intrare");
    }

    // Creeaza si deschide fisierul statistica.txt
    int stat_fd = open("statistica.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (stat_fd == -1) {
        afiseaza_eroare("Eroare la crearea fisierului statistic");
    }

    // Proceseaza directorul
    proceseaza_director(argv[1], stat_fd);

    // Inchide fisierele
    if (close(dir_fd) == -1 || close(stat_fd) == -1) {
        afiseaza_eroare("Eroare la inchiderea fisierelor");
    }

    return 0;
}
