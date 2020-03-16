/**************************
 * ex8b: Winning row- Threads
 * ============================================================================
 * The program creates 4 threads. The main program is the "main" thread that
 * initilaize a matrix with random numbers.
 * Each thread is in charge of a row in the
 * matrix. Each thread randomly generates numbers and checks in the number is
 * found in his row. If it is, it is replaced with zero.
 * The first row that fills with zeros first wins and ends the other threads
 * that are still running.
 *
 *
 * Written by: Tomer Fisher - 205364151, Shahar Aviv - 203489307
 * LogIn: Tomerfi, shaharav
 */

//--------------uncludes------------
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//----------defines------------------
#define NUM_OF_THREADS 4
#define N 20

//-----------Globals----------------------
pthread_once_t threads_kill = PTHREAD_ONCE_INIT;   // initialize behaivor
pthread_once_t threads_init = PTHREAD_ONCE_INIT;
pthread_key_t key;                                 // the value thread return
int result ;
int bingo[NUM_OF_THREADS][N];
pthread_t thread_data[NUM_OF_THREADS] ;
//----------prototypes--------------------

void rand_matrix() ;
void* my_func(void *) ;
void init_key() ;
int check_zero(const int row) ;
//----------------Main---------------------------------
int main()
{
	srand(17) ;
	int status ,i , arr[NUM_OF_THREADS];

	rand_matrix(); // make random matrix.

	for(i = 0 ; i < NUM_OF_THREADS ; i++)
	{
		arr[i] = i ;
		status =  pthread_create(&thread_data[i], NULL, my_func, (void *) &arr[i]);
		if (status != 0)
		{// failed to create
			fputs("thread create failed in  main", stderr);
			exit(EXIT_FAILURE);
		}
	}

	pthread_exit(0) ;
}
//-------------------------------------------------------
void rand_matrix()
{
	int i, j,num;
	for(i = 0 ; i < NUM_OF_THREADS ; i++)
	{
		for(j = 0 ; j < N ; j++)
		{
			num = rand() % 1000 + 1;
			bingo[i][j] = num;
		}
	}
}
//--------------------------------------------------------------
void *my_func(void * args)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	int row = *((int*)args) ;


	int i ;
	while(1){
		int num = rand() % 1000 + 1 ;//random number.
		for(i = 0 ; i < N ; i++)
		{
			if(bingo[row][i] == num)
				bingo[row][i] = 0;
		}


		if(check_zero(row) == 1){
			result = row;
			pthread_once(&threads_init, init_key);
			break;
		}

	}
	return NULL ;
}
//-------------------------------------------------------------
void init_key()
{

	int i ;
	for(i = 0 ; i < NUM_OF_THREADS ; i++)
	{
		if(i != result)
			pthread_cancel(thread_data[i]);
	}
	printf("%d\n", result) ;
	exit(EXIT_SUCCESS) ;
}
//-------------------------------------------------
int check_zero(const int row)
{

	int i ;
	for(i = 0 ; i < N ; i++)
	{
		if(bingo[row][i] != 0)
			return 0 ;
	}
	return 1 ;
}
