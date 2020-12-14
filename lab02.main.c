//lab2
//ne41459@zut.edu.pl
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>



char* tablica = NULL;
char* tablica2 = NULL;
int i = 0;
int rozmiar;
int x = 1;
int flag = 0;
void signald()
{
	x = 0;
}

void signale()
{
	printf("Sygnal zostal przechwycony 1 razy \n");
}

int main(int argc, char* argv[])
{

	sigset_t iset;
	sigemptyset(&iset);
	sigaddset(&iset, SIGTSTP);
	sigprocmask(SIG_BLOCK, &iset, NULL);


	struct sigaction act;
	act.sa_handler = &signald;
	sigaction(SIGINT, &act, NULL);

	struct sigaction ect;
	ect.sa_handler = &signale;
	sigaction(SIGTSTP, &ect, NULL);


	if (argc < 2)
	{
		printf("Brak argumentow \n");
		exit(0);
	}
	else
	{

		int n = strlen(argv[1]);

		if (n > 0)
		{
			tablica = malloc(2);
			tablica2 = malloc(n - 1);
			if (n == 1)
			{
				tablica[0] = argv[1][0];
				while (x == 1);

				printf("%d %c\n", getpid(), tablica[0]);
				exit(0);
			}
			else
			{
				tablica[0] = argv[1][0];
				tablica[1] = 0;

				for (i = 1; i < n; i++)
				{
					tablica2[i - 1] = argv[1][i];

				}
			}
		}
	}
	int stat = fork();
	if (stat == 0)
	{

		stat = setpgid(0, 0);
		execl(argv[0], argv[0], tablica, NULL);
	}
	else
	{

		int stat2 = fork();
		if (stat2 == 0)
		{
			stat2 = setpgid(0, 0);
			execl(argv[0], argv[0], tablica2, NULL);

		}
		else
		{

			while (x == 1);

			kill(stat, SIGINT);
			kill(stat2, SIGINT);
			stat = wait(NULL);
			stat2 = wait(NULL);


			printf("%d ", getpid());
			printf("%s\n", argv[1]);
		}
	}


	sigprocmask(SIG_UNBLOCK, &iset, NULL);
	free(tablica);
	free(tablica2);
	return 0;
}
