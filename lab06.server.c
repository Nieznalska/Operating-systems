#include <string.h>
#include<sys/types.h>
#include<stdio.h>
#include <ctype.h>
#include<sys/stat.h>
#include <getopt.h>
#include<stdlib.h>
#include <unistd.h>
#include<fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <errno.h>
#include <sys/un.h>
#include <netdb.h>
#include<sys/ioctl.h>
#include <signal.h>
#include <dirent.h>

extern int errno;

void usun_proc(char*nazwa_programu);
void zmien_wiadomosc(char* operation,char*stringtosend);

int main(int argc,char*argv[])
{
  struct pollfd fds[2];

  int ret;
  int pflag=0;
  int qflag=0;
  pid_t demon=0;
  int opt=0;
  int port=0;
  int sid;
  int my_server=0;
  int client;
  int endserver=0;
  struct sockaddr_in my_adress;
  int binds=0;
  int listenl=0;
  int current_size=0;
  char stringtosend[256];
  char operation[30];
  int recvsize=0;
  int nfds=1;
  int dflag=0;
  char operation2[20];
  current_size=nfds;

  while ((opt=getopt(argc,argv,"p:q")) !=-1)
  {
    switch (opt)
    {
      case'p':
      pflag=1;
      port=atoi(optarg);
      break;
      case 'q':
      qflag=1;
      break;
      case'?':
      printf("Nie znam tego\n");
      return 1;
    }
  }

   demon=fork();
    if(demon<0)
   {
      exit(1);
    }
    else if (demon>0)
    {
      exit (0);
    }
    sid=setsid();
    if(sid<0)
    {
      printf("Nie dziala setsid\n");
      exit(1);
    }

  int devnull=open("/dev/null",O_RDWR);
  close(0);
  close(1);
  close(2);
  dup2(devnull,0);
  dup2(devnull,1);
  dup2(devnull,2);
  close(devnull);
  //tutaj powienien sprawdzac czy musi zabic proces
  //flaga q



  // logika serwera
  if(qflag==1)
  {
    usun_proc(argv[0]);
  }
  if((my_server=socket(AF_INET,SOCK_STREAM,0))<0)
  {
    printf("Blad przy tworzeniu gniazda\n");
    exit(1);
  }
  int on=1;
  binds=ioctl(my_server,FIONBIO,(char*)&on);
  if(binds<0)
  {
    printf("Blad nie blokowania\n");
    exit(1);
  }


  my_adress.sin_family=AF_INET;
  my_adress.sin_addr.s_addr=INADDR_ANY;
  my_adress.sin_port=htons(port);//zamiana konwencji bitow
  int opcja=1;
  binds=setsockopt(my_server,SOL_SOCKET,SO_REUSEADDR,(void*)&opcja,sizeof(opcja));



  if(binds<0)
  {
    printf("Blad! ustawianie opcji serwera sie nie udalo\n%s\n", strerror(errno));
    close(my_server);
    exit(1);
  }


  struct addrinfo* res;
  char* service;
  sprintf(service, "%d", port);
  if (getaddrinfo("127.0.0.1", service, NULL, &res) != 0)
  {
    printf("getaddrinfo error\n");
    exit(1);
  }

  if((binds=bind(my_server, (struct sockaddr*)&my_adress,sizeof(my_adress)))<0)
  {
    printf("Blad przy wiazaniu adresu\n%s\n", strerror(errno));
    exit(1);
  }
  printf("%d",binds);
  if(pflag==1)
  {
    printf("[serv] jestem\n");

    listenl=listen(my_server,3);
    if(listenl<0)
    {
      printf("Blad nasluchiwania\n ");
      close(my_server);
      exit(1);
    }

    memset(fds,0,sizeof(fds));
    fds[0].fd=my_server;
    fds[0].events=POLLIN;
    int timeout=(3*60*1000); // Czas zakonczenia dzialania serwera jesli jest bezczynny
    int nfds=1;
    int gflag=1;

    while(gflag)
    {

      binds=poll(fds,nfds,timeout);
      if(binds<0)
      {
        printf("Blad poll\n");
        break;
      }
      else if(binds==0)
      {
        printf("Zakonczenie poll. Koniec aktywnosci serwera\n");
        break;

      }

      current_size=nfds;
      for(int i=0;i<current_size;i++)
      {
        printf("While z poll\n");
        if(fds[i].revents==0)
        {
          continue;
        }

        if(fds[i].revents!=POLLIN)
        {
          printf("Blad revents %d\n",fds[i].revents);
          endserver=1;
          break;
        }

        if(fds[i].fd==my_server)
        {
          printf("Serwer nasluchuje\n");
          do
          {
            client=accept(my_server,NULL,NULL);
            if(client<0)
            {
              if(errno!=EWOULDBLOCK)
              {
                printf("Blad accept");
              }
              break;
            }
            fds[nfds].fd=client;
            fds[nfds].events=POLLIN;
            fds[nfds].revents=POLLOUT;
            nfds++;
          }
          while(client!=-1);
        }
        else
        {
          dflag=0;
          do {
            if(i==0)
            {
              binds=recv(fds[i].fd,operation,sizeof(operation),0);
              printf("%d\n",i);

            }
            else
            {
              printf("%s\n",strerror(errno));
              binds=recv(fds[i].fd,stringtosend,sizeof(stringtosend),0);
              printf("%d\n",i);
            }

            printf("_____________***%s***_____________\n\n______________%d________________\n",operation,sizeof(operation));


            gflag=0;
            if(binds<0)
            {
              if(errno!=EWOULDBLOCK)
              {
                printf("recv nie dziala\n");
                dflag=1;
              }
              break;
            }
            if(binds==0)
            {
              printf("polaczenie zakonczone");
              dflag=1;
              break;
            }


            zmien_wiadomosc(operation,stringtosend);


            if((send(fds[i].fd,"cat",3,0))<0)
            {
              printf("Blad wysylania\n");
            }
            printf("%s  %s [klient]",operation,stringtosend);
          } while(1);


        }
      }
    }
  }

  if(dflag)
  {
    for(int i=0;i<nfds;i++)
    {
      if(fds[i].fd>=0)
      close(fds[i].fd);
    }

  }
  printf("[serv] zamykam sie\n");
  return 0;

}

void zmien_wiadomosc(char* operation,char*stringtosend)
{
//int flag1,flag2,flag3,=0;
  printf("%s\n [operation]",operation);
  printf("%s\n [stringtosend]",stringtosend);

char* schowek;
int len;
if((schowek=strstr(operation,"tolower"))!=NULL)
{
  len =strlen("tolower");
//  strcat(schowek,"tolower");
  printf("%s",operation);
}
if((schowek=strstr(operation,"toupper"))!=NULL)
{
  len =strlen("toupper");
//  strcat(schowek,"toupper");
    printf("%s",operation);
}
if((schowek=strstr(operation,"invert"))!=NULL)
{
  len =strlen("invert");
//  strcat(schowek,"invert");
  printf("%s",operation);
}

  printf("%s",operation);
printf("%s   [schowek]\n",schowek);
//printf("%s");

  if(strcmp(operation,"tolower")==0)
  {

    for(int i=0;i<strlen(stringtosend);i++)
    {
      stringtosend[i]=tolower(stringtosend[i]);
    }

    printf("%s",stringtosend);
    return;
  }
  else if(strcmp(operation,"toupper")==0)
  {
    for(int i=0;i<strlen(stringtosend);i++)
    {
      stringtosend[i]=  toupper(stringtosend[i]);
    }
    //printf("********%s",stringtosend);
    return;
  }
  else if(strcmp(operation,"invert")==0)
  {
    int len=strlen(stringtosend);
    char*result=malloc(len+1);
    printf("%d\ntest\n", len);
    int j=0;
    for(int i=0; i < len; i++)
    {
      result[len - i - 1]=stringtosend[i];
    }
    result[len] = '\0';
    return;
  }
  else
  {
    printf("Blad w funkcji\n");
    return;
  }

}



char plik[50];
int decision=0;




void sprawdzkatalog( const char* sciezka,char* namefile)
{
  char*sciazkadopliku=(char*)malloc(100*sizeof(char));
  sprintf(sciazkadopliku,"%sstatus",sciezka);
  FILE * f=fopen(sciazkadopliku,"r");
  size_t l=0;
  ssize_t read;
  char*odczyt=NULL;
  if(f!=NULL)
  {
    read=getline(&odczyt,&l,f);
    odczyt[read-1] = '\0';
    odczyt += 6;
    if(strcmp(namefile,odczyt)==0)
    {
      decision=1;
    }
  }
  fclose(f);
}
void usun_proc(char*nazwa_programu)
{
  DIR*dir;
  struct dirent *entry;
  char* namefile=malloc(sizeof(nazwa_programu)+1);
  strcpy(namefile, nazwa_programu);
  char idofprograms[1024];


  if((dir=opendir("/proc"))==NULL)
  {
    printf("Nie moge otworzyc katalogu");
    exit(0);
  }
  else
  {

    while((entry=readdir(dir))!=NULL)
    {

      if(entry->d_type==DT_DIR && isdigit(entry->d_name[0]))
      {
        char bufor[1024];
        if((strcmp(entry->d_name,".")==0))
        {
          continue;
        }
        else if((strcmp(entry->d_name,"..")==0))
        {
          continue;
        }
        sprintf(bufor,"/proc/%s/",entry->d_name);
        sprintf(idofprograms,"%s",entry->d_name);
        sprawdzkatalog(bufor,namefile);
        if(decision==1)
        {
          printf("Killall: %s\n",idofprograms);
          int ki;
          ki=atoi(idofprograms);
          pid_t prockilled=(pid_t)ki;

          if(kill(prockilled,SIGINT)==-1)
          {

            printf("Kill nie dziala");
          }
          else
          {
            continue;

          }
        }

      }
      else
      {grams[1024];

        continue;
      }

    }
    closedir(dir);

  }


  return;
}
