/**************************
 * ex4a3: the client
 * ============================================================================
 *
 * The program gets arguments then if it gets zero connects to gcd and 
 * if gets 1 connects to factor server. the client send the values to the relevant 
 * server got the result and print it
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisherr- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc -Wall ex4a3.c -o ex4a3
 * Run: ./ex4a3
 * ============================================================================
 * Input: from args of the client.
 * ============================================================================
 * Output: the calculation result from the relevant server
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

//----------defines------------------
#define MAX_TEXT 30
#define SIZE 100
#define ANSWER 2
#define NUM1 0
#define NUM2 1
int ALLOWED = -1;
//----------structs-------------------
struct my_msgbuf
{
    long m_type;
    int m_arr[SIZE];
};
//---------------globals----------------------
int msgid;
//-------------------------------------------
//-------------prototypes--------------------
void check_args(int argc ,char *argv[], char * c,struct my_msgbuf * my_msg);
void connect_to_server(char c,key_t * key,int * msgid);
void do_commend(int msgid,struct my_msgbuf * my_msg);
void print_arr(int arr[]);

//-------------main section-------------------

int main(int argc, char * argv[])
{

    struct my_msgbuf my_msg;
    key_t key;
    char c;


    check_args(argc,argv,&c,&my_msg);

    //printf("args are %d %d\n",my_msg.m_arr[0],my_msg.m_arr[1]);
    connect_to_server(c,&key,&msgid);
    do_commend(msgid,&my_msg);

    return 0;
}

//--------check the arguments and decide how to react----------------------
void check_args(int argc ,char *argv[] , char * c, struct my_msgbuf * my_msg)
{
    if(argc == 3) //for prime server
    {
        *c = 'd';
        my_msg->m_arr[0] = atoi(argv[2]);
	//puts("connect to factor");
 	my_msg->m_type = 13;
	ALLOWED = 7;
    }
    else if(argc == 4) //for gcd server
    {
        *c = 'g';
        my_msg->m_arr[0] = atoi(argv[2]);
        my_msg->m_arr[1] = atoi(argv[3]);
	//puts("connect to gcd");
	my_msg->m_type = 17;
	ALLOWED = 8;
    }
    else
    {
        perror("args not valid\n");
        exit(EXIT_FAILURE);
    }

}
//-------connect to server by 
void connect_to_server(char c, key_t * key, int * msgid)
{

    if ((*key = ftok(".", c))==-1)
    {
        perror("ftok faild");
        exit(EXIT_FAILURE);
    }
   // puts("client did ftok");

    if ((*msgid = msgget(*key, 0))==-1)
    {
        perror("msgget faild");
        exit(EXIT_FAILURE);
    }
}
//..............................................................
void do_commend(int msgid,struct my_msgbuf * my_msg)
{
    //printf("client starts command , type %ld\n",my_msg->m_type);

    if ((msgsnd(msgid, my_msg, sizeof(int)*SIZE , 0))==-1)
    {
        perror("msgsnd faild");
        exit(EXIT_FAILURE);
    }
    //puts("client sent msg");

    if ((msgrcv(msgid, my_msg, sizeof(int)*SIZE, (long)ALLOWED, 0))==-1)
    {
        perror("msgrcv faild");
        exit(EXIT_FAILURE);
    }
    puts("client got result:");
    print_arr(my_msg->m_arr);

}
//------------------------------------------------------------------
void print_arr(int arr[])
{
    int i;

    for(i=0; arr[i]!=-1; i++)
    {
        printf("%d\n", arr[i]);
    }
}

