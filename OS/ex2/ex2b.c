/***************************************************************************
File: ex2b.c
Signal Wars
============================================================================
Written by: Shahar Aviv- 203489307, Tomer Fisher 205364151
Login: shaharav, tomerfi

* The program creates a child using fork.
* Then both the child and the father run in an infinite loop, they generate a
* random number btw 0 and 7. if they got zero they print and quit.
* if they got a number btw 1 and 3 they then send a signal to other process.
* then we set an alarm for 5 seconds and pause.
* if the alarm went off we go to alarm_handler function print, and exit.
* if we got a signal b4 the alarm went off(other processed killed)
* then we go to signal_handler print and continue, advance counter
* if we get 10 signals, we print and exit that we surrender
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

//----------prototypes------------------
void catch_sigusr1(int sig_num);
void catch_alarm(int sig_num);
void catch_sigchild(int sig_num);

int counter=0;
//----------main section------------------

int main()
{
    signal(SIGUSR1, catch_sigusr1); //signal handler for SIGUSR1
    signal(SIGALRM, catch_alarm); //signal handler for SIGALRM

    pid_t pid;
    srand((unsigned)time(NULL)); // starting rand action

    pid = fork(); //make a child

    while (1)
    {
        if (pid < 0)
        {
            fputs("error in fork", stderr);
            exit(EXIT_FAILURE);
        }


        int randNum = (rand() % 7); //generate num between 0-6

        if (randNum == 0) // for child
        {
            printf("process %d ends\n", getpid());
            exit(0);
        }

        if (randNum <= 3 && randNum >= 1)
        {
            if (pid == 0)
                kill(getppid(), SIGUSR1); //sending signal to partner
            else
            {
                kill(pid, SIGUSR1); // sending signal to partner
            }
        }

        // starting alarm
        alarm(5);
        pause();
        alarm(0);

    }
}

//--------catch sigusr handler for SIGUSR----------------------
void catch_sigusr1(int sig_num)
{
    counter++;

    if (counter==10)
    {
        printf("process %d surrender\n", getpid());
        exit(EXIT_SUCCESS);
    }

    printf("process %d has a partner\n", getpid());
}

//--------catch alarm handler for ALRM---------------------------
void catch_alarm(int sig_num)
{
    printf("process %d was left alone, and quite\n", getpid());
    exit(EXIT_SUCCESS);
}

//--------catch sigchild handler for SIGCHLD----------------------
void catch_sigchild(int sig_num)
{
   waitpid(WAIT_ANY, NULL, 0);
}
