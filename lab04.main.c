#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#define rows 2000
#define columns 1000

struct pomoc
{
  int liczba_kolumn;
  int poczatek;
  int koniec;
  int**t;

};

struct timespec start,stop;
double czas;
int t[rows][columns];
int g[rows][columns];
FILE *dd;

void *oblicz(void* _pom)///struktura
{

  printf("Watek sie utworzyl\n");
  struct pomoc *pom=(struct pomoc* )_pom;//////////////////////////////////////////
  int wynik=0;
  printf("%i W watku\n",pom->poczatek );
  printf("%i w watku\n",pom->koniec);

  for(int o=pom->poczatek;o<pom->koniec;o++)
   {
     for (int l=0;l<columns;l++)
   {
         wynik=t[l][o]+t[l][o]+t[l+1][o+1]+t[l+1][o+1];
         g[l][o]=wynik;
      
       }
  }


}


int main(int argc,char*argv[])
{

  clock_t begin,end;
  begin=clock();
  if(clock_gettime(CLOCK_REALTIME,&start)==-1)
  {
    printf("Nie dziala czas");
    exit(1);
  }


  if(argc<2)
  {
    printf("Brak argumentow\n");
    exit(1);
  }
  else  if(clock_gettime(CLOCK_REALTIME,&start)==-1)
  {
    printf("Nie dziala czas");
    exit(1);
  }
  {

    if(argv[2]!=NULL)
    {
      int watki=0;
      watki=atoi(argv[2]);
      //  printf("%i wypisuje z atoi\n",watki);
      struct pomoc *pom=malloc(sizeof(struct pomoc)*watki);/////////////////////////
      //  printf("watki po atoi:  %d",watki);
      if((watki%2==0)&&(watki<9)||watki==1)
      {
        FILE *f=fopen(argv[1],"r+");

        if(f!=NULL)
        {

          size_t l=0;
          ssize_t read;
          char*odczyt=NULL;
          read=getline(&odczyt,&l,f);
          odczyt[read-1] = '\0';
          char korektor[]=" ";
          char*schowek=NULL;
          schowek=strtok(odczyt,korektor);
          int count=0;
          int y=0;
          int count2=0;

          while(odczyt!=NULL)
          {
            y=atoi(schowek);
            t[count][count2]=y;
        //    printf("%d*********%d ***** %d ******* %d\n",y,t[count][count2],count,count2);
            schowek=strtok(NULL,korektor);
            count2++;
            if(count2==2000)
            {
              break;
            }


          }
          count2=0;
          while((read=getline(&odczyt,&l,f))>0)
          {
            schowek=strtok(odczyt,korektor);
            count++;
            count2=0;
            while(odczyt!=NULL)
            {
            //  printf("A nie bo tutaj");
              y=atoi(schowek);
              t[count][count2]=y;
          //    printf("%d*********%d ***** %d ******* %d\n",y,t[count][count2],count,count2);
              schowek=strtok(NULL,korektor);
              count2++;
              if(schowek==NULL)
              break;
            }

          }



        }
        int liczba=watki;
        pthread_t thread [8];

        for (int i=0;i<watki;i++)
        {
          //printf("Tablica");
          pom[i].liczba_kolumn=2000/watki;
          pom[i].poczatek=0;
          pom[i].koniec=0;

        }
        if(watki==1)
        {
          printf("Sprawdza czy jest jeden watek\n");

          pom[0].koniec =rows;
          pom[0].poczatek=0;
          int x=pthread_create (&thread[0],NULL,oblicz,(void *)&pom[0]);
          printf("%d\n",x);
          if(x!=0)
          {
            printf("Nie utworzylem watku x \n");
          }
        }
        else
        {
          for (int l=0;l<watki;l++)
          {
          //  printf("watek %i\n",l);

            if (l==0)
            {

              pom[l].poczatek=0;
              pom[l].koniec=(rows/watki);
              printf("poczatek %i   watek: %d\n",pom[l].poczatek,watki);
              printf("koniec %i watek: %d\n",pom[l].koniec,watki);
            }
            else
            {
              pom[l].poczatek=pom[l-1].koniec;
              pom[l].koniec=pom[l-1].koniec+pom[l-1].koniec;
            //  printf("Poczatek:%i\n",pom[l].poczatek);
            //  printf("Koniec:%i\n",pom[l].koniec);
            }
            int p=pthread_create (&thread[l],NULL,oblicz,(void *)&pom[l]);

            if(p!=0)
            {
              printf("Nie utworzylem watku p: %i\n",l);
            }
          }
        }
        for (int i=0;i<watki;i++)
        {
          pthread_join(thread[i],NULL);
        }

        dd=fopen("wynik.txt","w+");
          if(!dd)
          {
            printf("Nie dziala plik");
            exit(1);
          }

          for(int r=0;r<1000;r++)
          {
            for(int lk=0;lk<500;lk++)
            {
            //  printf(" %d ***%d ****%d\n",g[r][lk],r,lk);
              fprintf(dd,"%d ",g[r][lk]);
            }
            fprintf(dd,"\n");

          }


          fclose(f);

      }

    }

  }

  if(clock_gettime(CLOCK_REALTIME,&stop)==-1)
  {
    printf("Blad w czasie stop");
    exit(1);
  }
  end=clock();
  czas=(double)(end)-(begin)/CLOCKS_PER_SEC;
  ///czas=0;
//  czas=(start.tv_sec-stop.tv_sec);
  printf("%lfs.\n",czas);


  return 0;
}
