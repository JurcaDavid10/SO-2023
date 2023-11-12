#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

int main(int argc, char *argv[])
{
    struct stat fileInfo;
    stat(argv[1], &fileInfo);

    if (argc != 2 || S_ISREG(fileInfo.st_mode) == 0)
    {
      perror("Usage ./program <fisier_intrare>\n");
      exit(-1);
    }

    int fIn,fOut;
    char buffer[BUFSIZ];

    if( (fIn = open(argv[1],O_RDONLY)) < 0 )
    {
      perror("eroare fisier intrare\n");
    }
    
    if (read(fIn, buffer, BUFSIZ) == -1) {
        perror("eroare citire header ");
        close(fIn);
        return -1;
    }
    

    struct stat fileStat;
    fstat(fIn,&fileStat);

    printf("Nume: %s\n", argv[1]);
    printf("Dimensiune: %lld \n", (long long)fileStat.st_size);
    printf("user id: %d\n", fileStat.st_uid);
    printf("Numar legaturi: %ld\n", fileStat.st_nlink);
    printf("Data ultimei modificari: %s",ctime(&fileStat.st_mtime));


    // Owner's permissions
    printf("Owner: ");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf("\n");
  

    // Group's permissions
    printf("Group: ");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf("\n");

    // Others' permissions
    printf("Others: ");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    if( (fOut = open("statistica.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 )
    {
      perror("eroare creare fisier \n");
    }

    char buffer2[BUFSIZ];
    sprintf(buffer2,"Nume: %s\n Dimensiune: %lld bytes\n user id: %d\n numar legaturi: %ld\n ultima modificare: %s\n",argv[1],(long long)fileStat.st_size,fileStat.st_uid, fileStat.st_nlink,ctime(&fileStat.st_mtime));



    if(write(fOut,buffer2,strlen(buffer2))==-1)
    {
      perror("eroare scriere fisier");
    }



    close(fIn);
    close(fOut);
    return 0;

}

