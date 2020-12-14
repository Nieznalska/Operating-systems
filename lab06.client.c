#include <string.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/stat.h>
#include <getopt.h>
#include<stdlib.h>
#include <unistd.h>
#include<fcntl.h>
#include <sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<errno.h>


int main(int argc,char*argv[])
{
  int port=0;
  char*adressip=NULL;
  char*operation=NULL;
  char*stringtosend=NULL;
  struct sockaddr_in server;
  char odpowiedz_servera[100];
  int opt=0;
  while ((opt=getopt(argc,argv,"a:p:s:o:")) !=-1)
  {
    switch (opt)
    {
      case'a':
      adressip=optarg;
      strcat(adressip,"\0");
      break;
      case 'p':
      port=atoi(optarg);
      break;
      case 's':
      stringtosend=optarg;
      break;
      case 'o':
      operation=optarg;
      strcat(operation,"\0");
      break;
      case'?':
      printf("Nie znam tego\n");
      return 1;
    }
  }

  printf("ip='%s', port='%d'\n", adressip, port);

  if((operation!=NULL)&&(stringtosend)!=NULL)
  {
    if(strcmp(operation,"tolower")==0||strcmp(operation,"toupper")==0||strcmp(operation,"invert")==0)
    {
      int my_client=0;
      int ret=0;
      if((my_client=socket(AF_INET,SOCK_STREAM,0))>0)
      {
        printf("myclient = %d\n", my_client);
        memset(&server,'\0',sizeof(server));
        server.sin_family=AF_INET;
        server.sin_port=htons(port);
        server.sin_addr.s_addr=inet_addr(adressip);
        if((ret=connect(my_client,(struct sockaddr*)&server,sizeof(server)))<0)
        {
          printf("%d\n",ret);
          printf("%s", strerror(errno));
          printf("[client]Blad polaczenia");
          exit(1);
        }


          printf("[ret]: %d\n",ret);

          size_t operationlen=strlen(operation);
          size_t stringlen=strlen(stringtosend);
          ssize_t sendtoserver=send(my_client,operation,operationlen,0);
           sendtoserver=send(my_client,stringtosend,stringlen,0);



          printf("Wyslalem operacje\n");

            printf("dochodzi do wiadomosci\n");

              if(sendtoserver<0)
              {
               printf("Nie moge wyslac stringa do przetworzenia\n");
              }



          if(recv(my_client,odpowiedz_servera,100,0)>0)
          {
            printf("%s",odpowiedz_servera);
            fflush(stdout);
          }

            printf("%s",odpowiedz_servera);

      }
      else
      {
        printf("Blad tworzenia gniazda");
        exit(1);
      }
    }

  }
  else
  {
    printf("zle wartosci");
    exit(1);
  }




  return 0;
}
