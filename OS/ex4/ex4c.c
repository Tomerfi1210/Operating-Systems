  
 /**************************
 * ex4c: generate all the numbers
 * ============================================================================
 *
 * father makes an array and inside it numbers to 2 children.
 * every child has its own array and checks if random number from father 
 * appers in his array. if the all numbers were found the child tell to father and 
 * print his array
 *
 * Running States:
 * Both children finish together.
 * One child finishes before the other child.
 *			
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc -Wall ex4c.c -o ex4c
 * Run: ./ex4c
 * ============================================================================
 * Input: no input
 * ============================================================================
 * Output: the array of the first child that finish
 * ============================================================================
 */

//.....................includes.....................
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

typedef enum {false,true} bool;
//.....................structs......................
struct my_msgbuf
{
    long m_type;
    int m_data;
    bool m_finished;
    bool m_dad_msg;
};
//.....................prototypes...................
void open_queue(int * msgid);
void do_son(int msgid , struct my_msgbuf my_msg);
bool check_array(bool checkarr[]);
void update_array(bool checkarr[],int arr[],int data);
void send_finish_msg(int msgid,struct my_msgbuf my_msg);
void talk_to_sons(int msgid,struct my_msgbuf * my_msg);
//.....................consts and defines...........
#define NUM_OF_CHILDREN 2
#define ARR_SIZE 10
#define RAND_SIZE 100
#define DAD_TYPE 17
#define SON_TYPE 13
//.....................main.........................
int main()
{

    struct my_msgbuf my_msg;
    int msgid, i ;
    pid_t pid;

    srand(time(NULL));
    open_queue(&msgid);

    my_msg.m_data = 4;
    my_msg.m_dad_msg = true;
    my_msg.m_finished = false;

    for (i = 0; i < NUM_OF_CHILDREN; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            do_son(msgid,my_msg);
            exit(EXIT_SUCCESS);
        }
    }

    while (1)
    {
        my_msg.m_data = rand() % RAND_SIZE;

        for (i = 0; i < NUM_OF_CHILDREN; i++)
        {
            talk_to_sons(msgid,&my_msg);
            if (my_msg.m_finished)
            {
                send_finish_msg(msgid,my_msg);
                break;
            }
        }
        if(my_msg.m_finished)
            break;
    }

    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}
//..............creation of the queue.............................
void open_queue(int * msgid)
{
    key_t key;
    if((key = ftok(".", 'c')) == -1)
    {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }
    if ((*msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0600)) == -1)
    {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }
}
//............do son actions......................................
void do_son(int msgid , struct my_msgbuf my_msg)
{
    int arr[ARR_SIZE],i;
    bool checkarr[ARR_SIZE];

    for (i = 0; i < ARR_SIZE; i++)
    {
        arr[i] = rand() % RAND_SIZE;
        checkarr[i] = false;
    }

    while (1)
    {
        if ((msgrcv(msgid, &my_msg, sizeof(struct my_msgbuf),SON_TYPE,0)) == -1)
        {
            perror("msgrcv failed\n");
            exit(EXIT_FAILURE);
        }

        update_array(checkarr,arr,my_msg.m_data);
        if(!check_array(checkarr))
            break;
        if(!my_msg.m_dad_msg)
            break;

        my_msg.m_type = DAD_TYPE;
        if ((msgsnd(msgid, &my_msg, sizeof(struct my_msgbuf),0)) == -1)
        {
            perror("msgrcv failed\n");
            exit(EXIT_FAILURE);
        }
    }

    if(my_msg.m_dad_msg)
    {
        my_msg.m_finished = true;
        my_msg.m_type = DAD_TYPE;
        if ((msgsnd(msgid, &my_msg, sizeof(struct my_msgbuf),0)) == -1)
        {
            perror("msgrcv failed\n");
            exit(EXIT_FAILURE);
        }
        printf("son %d finished numbers are:\n ",getpid());
        for (i = 0; i < ARR_SIZE; i++)
            printf("%d\n ",arr[i]);
    }
}
//..............checking the array ..............................
bool check_array(bool checkarr[])
{
    int i;
    bool ok = false;
    for (i = 0; i < ARR_SIZE; i++)
        if(!checkarr[i])
            ok = true;
    return ok;
}
//.............updae the bool arry.................................
void update_array(bool checkarr[] ,int arr[],int data)
{
    int i;
    for (i = 0; i < ARR_SIZE; i++)
        if (arr[i] == data)
            checkarr[i] = true;
}
//..........sending finish message ..................................
void send_finish_msg(int msgid,struct my_msgbuf my_msg)
{
    my_msg.m_dad_msg = false;
    my_msg.m_type = SON_TYPE;
    if ((msgsnd(msgid, &my_msg, sizeof(struct my_msgbuf),0)) == -1)
    {
        perror("msgrcv failed\n");
        exit(EXIT_FAILURE);
    }
}
//............comunication between children and father....................
void talk_to_sons(int msgid,struct my_msgbuf * my_msg)
{
    my_msg->m_type = SON_TYPE;
    if ((msgsnd(msgid, my_msg, sizeof(struct my_msgbuf),0)) == -1)
    {
        perror("msgrcv failed\n");
        exit(EXIT_FAILURE);
    }

    if ((msgrcv(msgid, my_msg, sizeof(struct my_msgbuf),DAD_TYPE,0)) == -1) {
        perror("msgrcv failed\n");
        exit(EXIT_FAILURE);
    }
}

