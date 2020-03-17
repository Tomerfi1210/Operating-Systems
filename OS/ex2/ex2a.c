/*
File: ex2a.c
Signals
============================================================================
Written by: Shahar Aviv- 203489307, Tomer Fisher 205364151
Login: shaharav, tomerfi

*The program has a master process that forks a child process.
*Before the child is born the father determines a signal handler which will be
*activated when the process will be sent
*The SIGUSR1 signal, and the second SIGTERM signal handler.a
*In the program there is a loop that rolls ten times in which the father and
*son must perform the following actions:
*A. The process goes to a random period of time for a maximum of four seconds.
*B. The second procedure sends the SIGUSR1 signal.
*third. The process counts how many signals it sends to the second process.
*The program will print the number of signals sent at the end.
*/



//---------------Include------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for fork()
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <wait.h>
//--------------Prototyes and Globles-------------
int counter_sig = 0,
count_recive = 0;

void catch_sig(int signum);
void catch_term(int signum);
void is_valid(pid_t status) ;
void catch_child(int sinum) ;
//----------------Main-----------------------------
int main()
{
	srand((unsigned)time(NULL));

	signal(SIGUSR1, catch_sig);
	signal(SIGTERM, catch_term);
	signal(SIGCHLD, catch_child);

	int i ;
	pid_t status = fork() ;
	is_valid(status);

	for(i = 0 ; i < 10 ; i++)
	{
		sleep(rand() % 4 + 1) ;
		counter_sig++;

		if(status == 0)
			kill(getppid(), SIGUSR1) ;

		else
			kill(status, SIGUSR1) ;

		if(counter_sig >= 2 && count_recive >= counter_sig*2)
		{
			printf("process <%d> surrender %d %d \n",getpid(), count_recive, counter_sig) ;
			if(status == 0)
				kill(getppid(), SIGTERM) ;
			else
				kill(status, SIGTERM);

			exit(EXIT_SUCCESS) ;
		}
	}

	return EXIT_SUCCESS ;
}
//-----------------------------------------
void catch_sig(int signum)
{
	signal(SIGUSR1, catch_sig);
	printf("process <%d> got signal SIGUSR1 \n", getpid()) ;
	count_recive++ ;
}
//-----------------------------------------
void catch_term(int signum)
{
	signal(SIGTERM, catch_term);
	printf("process <%d> win %d %d \n", getpid(),count_recive, counter_sig) ;
	exit(EXIT_SUCCESS) ;
}

//------------------------------------------------------------
void catch_child(int sinum)
{
	signal(SIGCHLD, catch_child) ;
	waitpid(-1, NULL, 0) ;
}
//----------------------------------------------------
void is_valid(pid_t status)
{
	if(status < 0)
	{
		perror("Cannot fork()");
		exit(EXIT_FAILURE) ;
	}
}

