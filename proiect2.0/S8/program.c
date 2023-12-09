#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dirent.h>
#include <string.h>


void afiseaza_eroare(char *mesaj) 
{
    perror(mesaj);
    exit(EXIT_FAILURE);
}

void afiseaza_utilizare() 
{
    fprintf(stderr, "Utilizare: ./program <director_intrare> <director_iesire>\n");
    exit(EXIT_FAILURE);
}

void conversieGri(char *cale_fisier)
{
    int bmp_fd = open(cale_fisier,O_RDWR); //deschidem fisierul pentru citire si scriere
    if (bmp_fd == -1) 
    {
        afiseaza_eroare("Eroare la deschiderea fisierului BMP"); //daca nu se deschide,afisam eroare
    }

    // Citeste dimensiunile imaginii BMP din header
    unsigned char header[54]; // Header BMP are 54 de octeti
    if (read(bmp_fd, header, sizeof(header)) != sizeof( header))
    {   //incerc sa citesc continutul header-ului (54octeti) din fisierul descriptor bmp_fd
        afiseaza_eroare("Eroare la citirea header-ului BMP"); //daca nr de octeti citit este diferit de dimensiunea care trebuie sa o aiba header-ul afiseaza mesaj de eroare
    }

    int latime = (int)&header[18]; //adresa de memorie a celui de-al 19-lea octet din header, care reprezinta inceputul informatiilor despre latimea imaginii . Adresa o convertimla int
    int inaltime  = (int)&header[22];//adresa de mem a celui de-al  23-lea octet , care repr inceputul informatiilor despre inaltime

    //parcurgere pixeli
    for(int i=0;i<inaltime;i++)
    {
        for(int j=0;j<latime;j++)
        {
            unsigned char pixel[3];
            if(read(bmp_fd,pixel,sizeof(pixel))!=sizeof(pixel))
            {
                afiseaza_eroare("Eroare la citirea pixelilor");
            }
            //conventie BGR(albastru-verde-rosu)
            unsigned char gri=(unsigned char)(0.299*pixel[2]+0.587*pixel[1]+0.114*pixel[0]);
            
            lseek(bmp_fd,-3,SEEK_SET);//mutam cursorul cu 3 octeti pentru a rescrie
            //rescrie pixeli
            write(bmp_fd, &gri, sizeof(gri));
            write(bmp_fd, &gri, sizeof(gri));
            write(bmp_fd, &gri, sizeof(gri));
        }
    }

    //inchide bmp
    close(bmp_fd);
}

void proceseaza_fisier_bmp(char *cale_fisier, int stat_fd) 
{
    // Deschide fisierul BMP in modul de citire
    int bmp_fd = open(cale_fisier, O_RDONLY);  //deschidere fisier pentru citire
    if (bmp_fd == -1) 
    {
        afiseaza_eroare("Eroare la deschiderea fisierului BMP"); //daca nu se deschide,afisam eroare
    }

    // Citeste dimensiunile imaginii BMP din header
    unsigned char header[54]; // Header BMP are 54 de octeti
    if (read(bmp_fd, header, sizeof(header)) != sizeof( header))
    {   //incerc sa citesc continutul header-ului (54octeti) din fisierul descriptor bmp_fd
        afiseaza_eroare("Eroare la citirea header-ului BMP"); //daca nr de octeti citit este diferit de dimensiunea care trebuie sa o aiba header-ul afiseaza mesaj de eroare
    }

    int latime = (int)&header[18]; //adresa de memorie a celui de-al 19-lea octet din header, care reprezinta inceputul informatiilor despre latimea imaginii . Adresa o convertimla int
    int inaltime = (int)&header[22];//adresa de mem a celui de-al  23-lea octet , care repr inceputul informatiilor despre inaltime

    // Creeaza sirul pentru drepturile de acces
    char drepturi_acces[10];
    sprintf(drepturi_acces, "%c%c%c",
            (header[16] & 4) ? 'R' : '-',       //bitul 4(2 la puterea 2) repr drept de citire
            (header[16] & 2) ? 'W' : '-',       //bitul 2 (2 la 1) repr drept de scriere
            (header[16] & 1) ? 'X' : '-');      //bitul 1(adica 2 la puterea 0) repr dreptul de executie

    // Creeaza sirul formatat si il scrie in fisier
    char *buffer_statistic = (char *)malloc(256);     //scrierea fisierului de statistica
     if (buffer_statistic == NULL)
    {
        afiseaza_eroare("Eroare la alocarea memoriei pentru bufferul statistic");
    }

    sprintf(buffer_statistic,       //stocam informatia in bufferul specificat pentru afisare
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
            lseek(bmp_fd, 0, SEEK_END),     //schimbam pozitia curenta de scriere a unui fisier deschis, si anume incepe de la sfarsitul fisierului
            0, 0, 0, // Valori temporare
            drepturi_acces,
            (header[14] & 4) ? 'R' : '-',
            (header[14] & 2) ? 'W' : '-',
            (header[14] & 1) ? 'X' : '-',
            (header[15] & 4) ? 'R' : '-',
            (header[15] & 2) ? 'W' : '-',
            (header[15] & 1) ? 'X' : '-');

    if (write(stat_fd, buffer_statistic, sizeof(buffer_statistic)) == -1) 
    {
        afiseaza_eroare("Eroare la scrierea in fisierul statistic"); //afisam eroare in cazul in care nu se poate scrie fisierul
    }

    free(buffer_statistic);

  /* pid_t pid_fiu = fork();
    if(pid_fiu==-1){
        afiseaza_eroare("Eroare creare proces fiu pt conversia img");
    } else if(pid_fiu==0){
        //proces fiu
        conversieGri(cale_fisier);
        exit(EXIT_SUCCESS);
    }

   //asteapta procesul fiu
    int status;
    if(waitpid(pid_fiu,&status,0)==-1){
        perror("Eroare la waitpid");
        exit(EXIT_FAILURE);
    }*/
    
    // Inchide fisierul BMP
    if (close(bmp_fd) == -1) 
    {
        afiseaza_eroare("Eroare la inchiderea fisierului BMP"); //afisam eroare in cazul in care nu se poate inchide
    }
}

void proceseaza_fisier_txt(char *nume_intrare,struct stat file_stat, int stat_fd)
{   
    // Creeaza sirul formatat si il scrie in fisier
    char buffer_statistic[256]; //stocam informatia in bufferul specificat pentru afisare
    sprintf(buffer_statistic,
                    "nume fisier: %s\n"
                    "dimensiune: %ld\n"
                    "identificatorul utilizatorului: %d\n"
                    "timpul ultimei modificari: %ld\n"
                    "contorul de legaturi: %ld\n"
                    "drepturi de acces user: %c%c%c\n"
                    "drepturi de acces grup: %c%c%c\n"
                    "drepturi de acces altii: %c%c%c\n",
                    nume_intrare, 
                    (long)file_stat.st_size, 
                    (int)file_stat.st_uid,
                    (long)file_stat.st_mtime, 
                    (long)file_stat.st_nlink,
                    (file_stat.st_mode & S_IRUSR) ? 'R' : '-',
                    (file_stat.st_mode & S_IWUSR) ? 'W' : '-',
                    (file_stat.st_mode & S_IXUSR) ? 'X' : '-',
                    (file_stat.st_mode & S_IRGRP) ? 'R' : '-',
                    (file_stat.st_mode & S_IWGRP) ? 'W' : '-',
                    (file_stat.st_mode & S_IXGRP) ? 'X' : '-',
                    (file_stat.st_mode & S_IROTH) ? 'R' : '-',
                    (file_stat.st_mode & S_IWOTH) ? 'W' : '-',
                    (file_stat.st_mode & S_IXOTH) ? 'X' : '-');

    // Scrie in fisierul statistica.txt
    if (write(stat_fd, buffer_statistic, strlen(buffer_statistic)) == -1) {
        afiseaza_eroare("Eroare la scrierea in fisierul statistica");
    }        
}

void proceseaza_tip_director(char *nume_intrare,struct stat file_stat, int stat_fd)
{  
    // Creeaza sirul formatat si il scrie in fisier
    char buffer_statistic[256]; //stocam informatia in bufferul specificat pentru afisare
    sprintf(buffer_statistic, 
                "nume director: %s\n"
                "identificatorul utilizatorului: %d\n"
                "drepturi de acces user: %c%c%c\n"
                "drepturi de acces grup: %c%c%c\n"
                "drepturi de acces altii: %c%c%c\n",
                nume_intrare, (int)file_stat.st_uid,
                (file_stat.st_mode & S_IRUSR) ? 'R' : '-',
                (file_stat.st_mode & S_IWUSR) ? 'W' : '-',
                (file_stat.st_mode & S_IXUSR) ? 'X' : '-',
                (file_stat.st_mode & S_IRGRP) ? 'R' : '-',
                (file_stat.st_mode & S_IWGRP) ? 'W' : '-',
                (file_stat.st_mode & S_IXGRP) ? 'X' : '-',
                (file_stat.st_mode & S_IROTH) ? 'R' : '-',
                (file_stat.st_mode & S_IWOTH) ? 'W' : '-',
                (file_stat.st_mode & S_IXOTH) ? 'X' : '-');

    // Scrie in fisierul statistic.txt
    if (write(stat_fd, buffer_statistic, strlen(buffer_statistic)) == -1) {
        afiseaza_eroare("Eroare la scrierea in fisierul statistica");
    }
}

void proceseaza_legatura_simbolica(char *cale_fisier, char *nume_intrare, struct stat file_stat, int stat_fd)
{
    
    // Obține informații despre tinta legaturii simbolice
    struct stat link_target_stat;
    if (lstat(cale_fisier, &link_target_stat) == -1) 
    {
        perror("Eroare la obtinerea informatiilor despre link-ul simbolic");
        exit(EXIT_FAILURE);
    }
    // Creeaza sirul formatat
    char buffer_statistic[256];
    if (S_ISREG(link_target_stat.st_mode)) 
    {
        // Construiește și scrie în fișierul statistica.txt
        sprintf(buffer_statistic,
                "nume legatura: %s\n"
                "dimensiune: %ld\n"
                "dimensiune fisier: %ld\n"
                "drepturi de acces user: %c%c%c\n"
                "drepturi de acces grup: %c%c%c\n"
                "drepturi de acces altii: %c%c%c\n",
                nume_intrare, 
                (long)file_stat.st_size, 
                (file_stat.st_mode & S_IRUSR) ? 'R' : '-',
                (file_stat.st_mode & S_IWUSR) ? 'W' : '-',
                (file_stat.st_mode & S_IXUSR) ? 'X' : '-',
                (file_stat.st_mode & S_IRGRP) ? 'R' : '-',
                (file_stat.st_mode & S_IWGRP) ? 'W' : '-',
                (file_stat.st_mode & S_IXGRP) ? 'X' : '-',
                (file_stat.st_mode & S_IROTH) ? 'R' : '-',
                (file_stat.st_mode & S_IWOTH) ? 'W' : '-',
                (file_stat.st_mode & S_IXOTH) ? 'X' : '-');

        // Scrie in fisierul statistic.txt        
        if (write(stat_fd, buffer_statistic, strlen(buffer_statistic)) == -1) 
        {
            perror("Eroare la scrierea în fișierul statistica");
            exit(EXIT_FAILURE);
        }
    }
}

void proceseaza_intrare(char *nume_intrare, char *cale_parinte, int stat_fd) {
    char cale_completa[256];
    sprintf(cale_completa, "%s/%s", cale_parinte, nume_intrare);

    pid_t pid = fork();

    if (pid == -1) 
    {
        perror("Eroare la crearea procesului fiu.");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) // Proces copil
    {  
        struct stat file_stat;
        if (lstat(cale_completa, &file_stat) == -1) 
        {
            afiseaza_eroare("Eroare la obtinerea informatiilor despre fisier");
            exit(EXIT_FAILURE);
        }

        // Apelarea functiei corecte de processare in functie de datele de intrare
        if (S_ISREG(file_stat.st_mode)) 
        {
            if (strstr(nume_intrare, ".bmp") != NULL) 
            {
                proceseaza_fisier_bmp(cale_completa, stat_fd);
            } else {
                proceseaza_fisier_txt(nume_intrare, file_stat, stat_fd);
            }
        } 
        else if (S_ISDIR(file_stat.st_mode)) 
        {
            proceseaza_tip_director(nume_intrare, file_stat, stat_fd);
        } 
        else if (S_ISLNK(file_stat.st_mode)) 
        {
            proceseaza_legatura_simbolica(cale_completa, nume_intrare, file_stat, stat_fd);
        }

        printf("S-a incheiat procesul cu pid-ul %d ",getpid());

        exit(EXIT_SUCCESS);
    } 
    else // Proces parinte
    { 
        // Asteapta procesul copil 
        int status;
        if (waitpid(pid, &status, 0) == -1) 
        {
            perror("Eroare la asteptarea copilului");
            exit(EXIT_FAILURE);
        }
    }
}

void proceseaza_director(char *cale_director, int stat_fd) 
{
    DIR *director = opendir(cale_director);
    if (!director) 
    {
        afiseaza_eroare("Eroare la deschiderea directorului");
    }

    struct dirent *intrare;
    while ((intrare = readdir(director)) != NULL) 
    {
        if (strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0) 
        {
            proceseaza_intrare(intrare->d_name, cale_director, stat_fd);
        }
    }

    closedir(director);
}

int main(int argc, char *argv[]) {
    // Verifica numarul corect de argumente
    if (argc != 3) 
    {
        afiseaza_utilizare();
    }

    char *director_intrare = argv[1];
    char *director_iesire = argv[2];

    // Deschide directorul in modul de citire
    int dir_fd = open(director_intrare, O_RDONLY);
    if (dir_fd == -1) 
    {
        afiseaza_eroare("Eroare la deschiderea directorului de intrare");
    }

    // Construieste calea completa pentru fisierul de statistica in directorul de iesire
    char statistica_path[256];
    sprintf(statistica_path, "%s/%s_statistica.txt", director_iesire, director_intrare);

    // Creeaza si deschide fisierul de statistica in directorul de iesire
    int stat_fd = open(statistica_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (stat_fd == -1) 
    {
        afiseaza_eroare("Eroare la crearea fisierului statistic");
    }

    // Proceseaza directorul
    proceseaza_director(director_intrare, stat_fd);

    // Inchide directorul si fisierul
    if (close(dir_fd) == -1 || close(stat_fd) == -1) 
    {
        afiseaza_eroare("Eroare la inchiderea directorului sau a fisierului");
    }

    return 0;
}