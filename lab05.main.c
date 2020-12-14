#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <dirent.h>
#include <ftw.h>
#include <signal.h>
#include <ctype.h>
#include <stdbool.h>
#include <signal.h>
#include<errno.h>

char plik[50];
bool decision=false;




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
			decision=true;
		}
	}
	fclose(f);
}

bool iskillall=false;

int main(int argc,char*argv[])
{
	if(argc == 2)
	{
		ssize_t linkowanie;
		size_t buforowanie;
		char*buf=(char*)malloc(100*sizeof(argv[0]));
		if((linkowanie=readlink(argv[0],buf,sizeof(buf)))<0)
		{
			printf("%d\n",errno);
		}
		if(strcmp(buf,"pidof")==0)
		{
			iskillall=true;
		}

		DIR*dir;
		struct dirent *entry;
		char* namefile=malloc(sizeof(argv[1])+1);
		strcpy(namefile, argv[1]);
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
					if(decision==true)
					{
						if(iskillall==false)
						{
							printf("%s\n",idofprograms);
						}

						else
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


				}
				else
				{
					continue;
				}

			}
			closedir(dir);

		}

	}
	else
	{
		printf("Brak nazwy programu.");
		exit(0);
	}

	return 0;
}
