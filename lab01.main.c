//lab1
//niez
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

char*tablica=NULL;
char*tablica2=NULL;
int i=0;
int rozmiar;
pid_t dziecko;
pid_t rodzic;
int main(int argc,char*argv[])
{

  if (argc<2)
  {
    printf("Brak argumentow \n");
    exit(0);
  }
  else
  {
    int n=strlen(argv[1]);

    if (n>0)
    {
      tablica=malloc(2);
      tablica2=malloc(n-1);
      if (n==1)
      {
      tablica[0]=argv[1][0];
      printf("%d %c\n",getpid(),tablica[0]);
      exit(0);
      }
      else
      {
      tablica[0]=argv[1][0];
      tablica[1]=0;

        for (i=1;i<n;i++)
        {
         tablica2[i-1]=argv[1][i];

        }
    }
  }
 }
int stat=fork();
if (stat==0)
{
execl(argv[0],argv[0],tablica,NULL);
}
else
{
  int stat2=fork();
  if (stat2==0)
  {
    execl(argv[0],argv[0],tablica2,NULL);
  }
  else
  {
stat=wait(NULL);
  if (stat==-1)
  {
  printf("Tutaj jest blad");
  }
stat2=wait(NULL);
if (stat2==-1)
{
printf("Tutaj jest blad");
}

printf("%d ",getpid());
printf("%s \n",argv[1]);
  }
}

free(tablica);
free(tablica2);
  return 0;
}//lab1
//ne41459@zut.edu.pl
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

char*tablica=NULL;
char*tablica2=NULL;
int i=0;
int rozmiar;
pid_t dziecko;
pid_t rodzic;
int main(int argc,char*argv[])
{

  if (argc<2)
  {
    printf("Brak argumentow \n");
    exit(0);
  }
  else
  {
    int n=strlen(argv[1]);

    if (n>0)
    {
      tablica=malloc(2);
      tablica2=malloc(n-1);
      if (n==1)
      {
      tablica[0]=argv[1][0];
      printf("%d %c\n",getpid(),tablica[0]);
      exit(0);
      }
      else
      {
      tablica[0]=argv[1][0];
      tablica[1]=0;

        for (i=1;i<n;i++)
        {
         tablica2[i-1]=argv[1][i];

        }
    }
  }
 }
int stat=fork();
if (stat==0)
{
execl(argv[0],argv[0],tablica,NULL);
}
else
{
  int stat2=fork();
  if (stat2==0)
  {
    execl(argv[0],argv[0],tablica2,NULL);
  }
  else
  {
stat=wait(NULL);
  if (stat==-1)
  {
  printf("Tutaj jest blad");
  }
stat2=wait(NULL);
if (stat2==-1)
{
printf("Tutaj jest blad");
}

printf("%d ",getpid());
printf("%s \n",argv[1]);
  }
}

free(tablica);
free(tablica2);
  return 0;
}
