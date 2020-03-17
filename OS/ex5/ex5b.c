 /**************************
 * ex5b: generate all my numbers
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
 * Compile: gcc -Wall ex5b.c -o ex5b
 * Run: ./ex5b
 * ============================================================================
 * Input: no input
 * ============================================================================
 * Output: the array of the child that finish first
 * ============================================================================
 */

//.....................includes.....................
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

//---------------defines-------------------------
#define NUM_OF_CHILD 2
#define MAX_RAND 100
//-------------globals----------------------------
int shm_id;
//------------prototypes--------------------------
void do_random(int arr[]);
void do_child();
int find_num(int num, int arr[]);
void do_finish(int arr[], int child_kind);
void print_arr(int arr[]);
void catch_child1(int signum);
void catch_child2(int signum);
void catch_term(int signum);

//----------------main section------------------------

int main()
{
	signal(SIGUSR1, catch_child1);
	signal(SIGUSR2, catch_child2);
	//signal(SIGTERM,catch_term);

	int* father_ptr;
	key_t key;
	int i;
	pid_t pid;

	if ((key = ftok("./ex5b.c", 'a')) == -1) // do ftok
	{
		perror("ftok failed");
		exit(EXIT_FAILURE);
	}

	if ((shm_id = shmget(key, 16, IPC_CREAT | IPC_EXCL | 0600)) == -1) // creation of shared memory
	{
		perror("failed to get memory\n");
		exit(EXIT_FAILURE);
	}

	if ((father_ptr = (int*)shmat(shm_id, NULL, 0)) < 0) // creation of pointer to shared memory
	{
		perror("failed to attach memory");
		exit(EXIT_FAILURE);
	}
	//("dad pid is %d\n",getpid());

	for (i=0; i < NUM_OF_CHILD; i++) // creation of 2 children
	{
		pid = fork();
		srand((unsigned) time(NULL)+getpid());

		if (pid == 0)
		{
			do_child(i);
			i++;
		}
		father_ptr[i] = pid;	//saving the child pid in fathers array
	}
	do_random(father_ptr);


	return EXIT_SUCCESS;
}

//-----------------creation of random numbers for children-----------------------
void do_random(int arr[])
{
	arr[2] = arr[3] = 0;

	//puts("dad starts random");

	while (1)
	{
		int num;
		if(arr[2]==0 && arr[3]==0)
		{

			num = (rand()% MAX_RAND) + 1;
			arr[2]=num;
			arr[3]=num;
		}
	}
}
//----------------missions of child--------------------------------
void do_child(child_kind)
{
	//printf("child %d start\n", getpid());

	int arr[10];
	int i,j;
	int *child_ptr;
	bool check_arr[10];
	int counter=0;

	for(i=0; i<10; i++)
		arr[i]= (rand()% MAX_RAND) + 1; // put random values in the array

	//printf("child %d arr is: ",getpid());
	//for(i=0; i<10; i++)
	//	printf("%d ",arr[i]);
	//printf("\n");

	if (memset(&check_arr, 0, 10 * sizeof(bool)) == (int*)-1) // initializing the check_arr
			{
				perror("filed to memset");
				exit(EXIT_FAILURE);
			}

	//printf("child %d did random\n", getpid());

	if ((child_ptr = (int*)shmat(shm_id, NULL, 0)) < 0) // pointer to the array
	{
		perror("filed to attach memory");
		exit(EXIT_FAILURE);
	}
	//printf("child %d did shmat\n", getpid());


	while(1)
	{
		int num;

		num = child_ptr[child_kind+2];
		if(num != 0)
		{
			j = find_num(num, arr);

			if(j != -1 && check_arr[j] != true)
			{
				check_arr[j] = true; // to know that child found the number
				counter++;	// counting the number of numbers that were found
			}
			if(counter == 10) // when child found all the numbers
				do_finish(arr, child_kind);
		}

		child_ptr[child_kind+2] = 0; // put 0 in fathers array
	}
}

//-------------finding the value place in  array---------------------------
int find_num(int num, int arr[])
{
	int i;

	for(i=0; i<10; i++)
		if(arr[i] == num)
			return i;

	return -1;
}

//--------------finish actions for children----------------------------
void do_finish(int arr[], int child_kind)
{
	//printf("child %d finished\n",getpid());

	if(child_kind == 0)
	{
		kill(getppid(), SIGUSR1);
		print_arr(arr);
	}

	if(child_kind == 1)
	{
		kill(getppid(), SIGUSR2);
		print_arr(arr);
	}

	exit(EXIT_SUCCESS);
}

//-------------------printing the array----------------------------------
void print_arr(int arr[])
{
	//printf("child %d arr is: ", getpid());
	int i;
	for(i=0; i<10; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

//----------------signal handler for SIGUSR1--------------------------------
void catch_child1(int signum)
{
	//printf("dad %d catch child1\n",getpid());

	int* catch_ptr;
	if ((catch_ptr = (int*)shmat(shm_id, NULL, 0)) < 0)
	{
		perror("failed to attach memory");
		exit(EXIT_FAILURE);
	}
	kill((pid_t)(catch_ptr[1]), SIGKILL); // stoping the second child 
	if (shmctl(shm_id, IPC_RMID, NULL) == -1)
			{
				perror("faild to shmctl");
				exit(EXIT_FAILURE);
			}
	//puts("memory deleted");

	wait(NULL); // waiting for children
	wait(NULL);

	exit(EXIT_SUCCESS);
}

//----------------signal handler for SIGUSR2--------------------------------
void catch_child2(int signum)
{
	//printf("dad %d catch child2\n",getpid());
	int* catch_ptr;
	if ((catch_ptr = (int*)shmat(shm_id, NULL, 0)) < 0)
	{
		perror("failed to attach memory");
		exit(EXIT_FAILURE);
	}
	kill((pid_t)catch_ptr[0], SIGKILL); // stoping the second child

	if((shmctl(shm_id, IPC_RMID, NULL)) == -1)
	{
		perror("shmctl failed\n");
		exit(EXIT_FAILURE);
	}
	//puts("memory deleted");

	wait(NULL); // waiting for children
	wait(NULL);

	exit(EXIT_SUCCESS);
}
//.......................................................................
//void catch_term(int signum)
//{
//	if((shmctl(shm_id, IPC_RMID, NULL)) == -1)
//	{
//		perror("shmctl failed\n");
//		exit(EXIT_FAILURE);
//	}
//	puts("memory deleted");
//}

