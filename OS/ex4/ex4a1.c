/**************************
 * ex4a1: GCD Server
 * ============================================================================
 *
 * gcd server creat a queue and then runs infinitly,
 * waits for input from client. waiting for result from gcd algo and then 	
 * send it to client
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc -Wall ex4a1.c -o ex4a1
 * Run: ./ex4a1
 * ============================================================================
 * Input: got 2 numbers from client
 * ============================================================================
 * Output: send gcd result back to client
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

//----------defines---------------------
#define MAX_TEXT 30
#define SIZE 100
#define ANSWER 2
#define NUM1 0
#define NUM2 1
//----------structs----------------------
struct my_msgbuf
{
    long m_type;
    int m_arr[SIZE];
};
//-------------globals--------------------
int msgid;
//------------prototypes-------------------
int gcd(int num1, int num2);
void catch_sigint(int sig_num);
//----------main section------------------

int main()
{

    signal(SIGINT, catch_sigint); //signal handler for SIGINT
    struct my_msgbuf my_msg;
    key_t key;
    long type = 17; //

    if ((key = ftok(".", 'g'))==-1) //do ftok
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
       // puts("server starts loop");
        if ((msgrcv(msgid, &my_msg, sizeof(int)*SIZE, type, 0))==-1) //receiving messages from queue
        {
            perror("msgrcv faild");
            exit(EXIT_FAILURE);
        }

        //printf("server got msg type %ld\n",my_msg.m_type);
       // printf("server got %d %d\n",my_msg.m_arr[0],my_msg.m_arr[1]);

        int s = gcd(my_msg.m_arr[0],my_msg.m_arr[1]); //saving the gcd result in variable 
       // printf("the gcd is %d\n",s);

        my_msg.m_arr[0]= s;  //
	my_msg.m_arr[1]= -1; 
	my_msg.m_type = 8;
        if ((msgsnd(msgid, &my_msg, sizeof(int)*SIZE, 0))==-1) //send the gcd results to queue
        {
            perror("msgsnd faild");
            exit(EXIT_FAILURE);
        }

    }

}

//-------calculating the gcd of 2 numbers------------
int gcd(int num1, int num2)
{
    int i,
            gcd = -1;

    for(i=1; i <= num1 && i <= num2; ++i)
    {
        // Checks if i is factor of both integers
        if(num1%i==0 && num2%i==0)
            gcd = i;
    }

    return gcd;
}

//---------------signal handler for SIGINT---------------
void catch_sigint(int sig_num)
{
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
	printf("%d is removed\n", msgid);
        perror("msgctl() faild");
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
