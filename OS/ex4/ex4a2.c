/**************************
 * ex4a2: Prime Factor Server
 * ============================================================================
 *
 * the prime factor server creat a queue and run,
 * waits for input from client. the server calculate the prime factors by the 
 * prime factor algo and then writes the answer back to client
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc -Wall ex4a2.c -o ex4a2
 * Run: ./ex4a2
 * ============================================================================
 * Input: gets prime from client
 * ============================================================================
 * Output: send prime factors back to client
 * ============================================================================
 */

//---------------Include------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <math.h>


//----------defines------------------
#define MAX_TEXT 30
#define SIZE 100
#define ANSWER 2
#define NUM1 0
#define NUM2 1
//--------------structs-------------------
struct my_msgbuf
{
    long m_type;
    int m_arr[SIZE];
};
//--------------globals--------------------
int msgid;

//------------prototypes------------------
void primeFactors(int m_arr[]);
void catch_sigint(int sig_num);

//----------main section------------------
int main()
{

    signal(SIGINT, catch_sigint); //signal handler for SIGINT
    struct my_msgbuf my_msg;
    key_t key;
    long type = 13;

    if ((key = ftok(".", 'd'))==-1) // put 'g' instead of 'd'
    {
        perror("ftok faild");
        exit(EXIT_FAILURE);
    }
    //puts("server did ftok");

    if ((msgid = msgget(key, IPC_CREAT| IPC_EXCL| 0600))==-1) // creation of message queue
    {
        perror("msgget faild");
        exit(EXIT_FAILURE);
    }
   // puts("server did msgget");

    while(1)   
    {
      //  puts("server starts loop");
        if ((msgrcv(msgid, &my_msg, sizeof(int)*SIZE, type, 0))==-1) //receiving messages from queue
        {
            perror("msgrcv faild");
            exit(EXIT_FAILURE);
        }

       // printf("server got msg type %ld\n",my_msg.m_type);
       // printf("server got %d %d\n",my_msg.m_arr[0],my_msg.m_arr[1]);

        primeFactors(my_msg.m_arr); // calculate prime factors for number

	my_msg.m_type = 7;
        if ((msgsnd(msgid, &my_msg, sizeof(int)*SIZE, 0))==-1) // send the result to the queue
        {
            perror("msgsnd faild");
            exit(EXIT_FAILURE);
        }
       // printf("server sent msg %d",my_msg.m_arr[1]); 

    }

}

//-----------calculate prime factors for number-----------------
void primeFactors(int m_arr[])
{
    int n = m_arr[0];
    int i =0;
    int j;
    // Print the number of 2s that divide n
    while (n%2 == 0)
    {
        m_arr[i] = 2;
        n = n/2;
        i++;
    }

    // n must be odd at this point.  So we can skip
    // one element (Note i = i +2)
    for (j = 3; j <= n/2; j = j+2)
    {
        // While i divides n, print i and divide n
        while (n%j == 0)
        {
            m_arr[i] = j;
            n = n/j;
            i++;
        }
    }

    // This condition is to handle the case when n
    // is a prime number greater than 2
    if (n > 2)
    {
        m_arr[i] = n;
        i++;
    }

    m_arr[i] = -1;
}





//---------------signal handler for SIGINT------------------
void catch_sigint(int sig_num)
{
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl() faild");
        exit(EXIT_FAILURE);
    }
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
