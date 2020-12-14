// ./lab03 -f or -s or -n
//gcc lab03.main.c -o;
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

int main(int argc,char*argv[])
{

  int opt=0;
  int fflag=0;
  int nflag=0;
  int sflag=0;

  int ile_wierszy=10;
  int ile_sek;
  struct stat buf;
  int status;
  int des;
  char c;

  while ((opt=getopt(argc,argv,"n:fs:")) !=-1)
  {
    switch (opt)
    {
      case 'n':
      nflag=1;
      ile_wierszy=atoi(optarg);
      break;
      case 'f':
      fflag=1;
      break;
      case 's':
      sflag=1;
      ile_sek=atoi(optarg);
      break;
      case'?':
      printf("Nie znam tego\n");
      return 1;
    }
  }
  int index=optind;
  printf("%s\n",argv[index]);
  des=open(argv[index],O_RDWR);
  if (des==-1)
  {
    printf("errno=%d",errno);
  }
  FILE *f=fdopen(des,"r+");
  int przec=0;
  int pos;
  if (nflag==1)
  {
    fseek(f,0,SEEK_END);
    pos=ftell(f);
    if(pos==-1)
    {
      printf("lseek psuje");
    }

    pos=pos-1;
    while (pos)
    {

      if(!fseek(f,pos,SEEK_SET))
      {
        c=fgetc(f);

        if (c=='\n')
        {
          przec++;
          if (przec==ile_wierszy)
          {

            break;
          }
        }

      }
      pos=pos-1;
    }
    while(1){
      c=fgetc(f);
      if(c==EOF)
      {
        break;
      }
      printf("%c",c);
      if (c=='\n')
      {
        printf("\n");
      }
    }

  }
  if(fflag==1){


    struct timespec spr2;
    clock_gettime(CLOCK_REALTIME,&spr2);
    fstat(des,&buf);

    struct timespec spr;
    memcpy(&spr, &buf.st_mtime, sizeof(spr));

    while(1)
    {
      if(spr.tv_sec!=spr2.tv_sec)
      {



        fseek(f,0,SEEK_END);
        pos=ftell(f);
        if(pos==-1)
        {
          printf("lseek psuje");
        }

        pos=pos-1;
        while (pos)
        {
          if(!fseek(f,pos,SEEK_SET))
          {
            c=fgetc(f);
            if (c=='\n')
            {
              przec++;
              if (przec==ile_wierszy)
              {
                break;
              }
            }

          }
          pos=pos-1;
        }
        while(1){
          c=fgetc(f);
          if(c==EOF)
          {
            break;
          }
          printf("%c",c);
          if (c=='\n')
          {
            printf("\n");
          }
        }
      }


      if (sflag==1)
      {
        sleep(ile_sek);
      }
      if (fstat(des,&buf)==-1)
      {
        perror("lstat");
        exit(EXIT_FAILURE);
      }

      spr2.tv_sec=spr.tv_sec;
      memcpy(&spr, &buf.st_mtime, sizeof(spr));

    }
  }

  fclose(f);
  return 0;
}
