/**************************
 * ex5a: client and server- factorization and gcd
 * ============================================================================
 * The program has a client with two servers. Each server creates a shared 
 * memory, and the process front_end connects to both. The front_end waits
 * until he receives a character from the user and then accordingly activates 
 * the server needed. One server calculates the gcd
 * of two numbers, and the other server calculates the prime factorization
 * of a number. Both the data received from the user and the results are 
 * written on the shared memory. When the server finishes the calculation,
 * it sends a signal to the front_end who then prints out the results.
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc -Wall ex5a.c -o ex5a
 * Run: ./ex5a
 * ============================================================================
 * Input: From client. Characters g or d represents the server that the client 
 * wants to activate. The user then iserts the amount of numbers accordingly.
 * ============================================================================
 * Output: The result of calculation done by the server.
 * ============================================================================
 */

//.....................includes.....................
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <unistd.h>
#include <wait.h>

#define CHILDREN_SIZE 3
//--------------globals---------------------------
int  gcd_shm =- 1, prime_shm=-1;
int shm_id;
int *prime_ptr, *gcd_ptr ;
pid_t *pid_arr;
pid_t fatherpid;
//.....................prototypes...................
void check_key(key_t key) ;
int gcd(int n1,int n2) ;
void is_valid(pid_t status) ;
void check_ptr(int *ptr) ;
void catch_sig1(int s) {} ;
void catch_sig2(int s) {} ;
void primeFactors(int n, int *arr) ;
void do_gcd() ;
void do_primefactor() ;
void do_client() ;
void signal_handler(int num) ;

//.....................main.........................
int main()
{
	int i;
	int child;
	pid_t status ;

	key_t key = ftok(".", 'f') ;
	check_key(key) ;

	if ((shm_id = shmget(key,CHILDREN_SIZE*sizeof(pid_t), IPC_CREAT | IPC_EXCL | 0600)) == -1) // creation of shared memory
	{ 
		perror("shmget failed: ") ;
		exit(EXIT_FAILURE) ;
	}

	pid_arr = (pid_t*) shmat(shm_id, NULL, 0);
	check_ptr(pid_arr);

	fatherpid = getpid();
	for(child = 0 ; child < CHILDREN_SIZE ; child++)
	{
		status = fork() ;
		is_valid(status) ;

		if(status == 0)
		{
			((pid_t*)shmat(shm_id,NULL,0))[child] = getpid(); // continue by identity of child
			switch(child)
			{
			case 0 :
				signal(SIGINT, signal_handler) ;
				do_gcd() ;
				break;

			case 1:
				signal(SIGINT, signal_handler) ;
				do_primefactor() ;
				break;

			default:
				do_client() ;
				break ;
			}
		}
	}
	signal(SIGINT, signal_handler) ;
	for(i=0;i<CHILDREN_SIZE;i++)
		wait(NULL);

	return EXIT_SUCCESS ;

}
//------------------do client missions--------------------------------------
void do_client()
{
	int n1, n2 ;
	char choice ;
	while(1)
	{
		do{
		printf("Please enter g for GCD or d for prime \n") ;
		scanf(" %c", &choice) ;
		}while(choice != 'g' && choice != 'd') ;

		switch(choice)  // continue by the correct type
		{
		case 'g':
		{
			key_t key = ftok(".", 'g') ;
			check_key(key) ;

			if((gcd_shm = shmget(key, 0, 0600)) == -1) {
				perror("shmget failed: ") ;
				exit(EXIT_FAILURE) ;
			}
			gcd_ptr = (int *) shmat(gcd_shm, NULL, 0);
			check_ptr(gcd_ptr);

			printf("Please enter two numbers\n") ;
			scanf("%d %d", &n1, &n2) ;
			gcd_ptr[0] = n1;
			gcd_ptr[1] = n2;
			kill(((pid_t*)shmat(shm_id,NULL,0))[0] , SIGUSR1) ;
			signal(SIGUSR2, catch_sig2) ;
			pause() ;
			printf("result : %d\n", gcd_ptr[2]) ;
			break ;
		}
		case 'd':
		{
			key_t key = ftok(".", 'd') ;
			check_key(key) ;

			if((prime_shm = shmget(key, 0, 0600)) == -1) {
				perror("shmget failed: ") ;
				exit(EXIT_FAILURE) ;
			}
			prime_ptr = (int *) shmat(prime_shm, NULL, 0);
			check_ptr(prime_ptr);

			printf("Please enter one numbers\n") ;
			scanf("%d", &n1) ;
			prime_ptr[0] = n1;
			kill(((pid_t*)shmat(shm_id,NULL,0))[1] , SIGUSR1) ;
			signal(SIGUSR2, catch_sig2) ;
			pause() ;
			int i = 1 ;
			//print to see
			while(prime_ptr != NULL && prime_ptr[i] != -1)
			{
				printf("result : %d\n", prime_ptr[i]) ;
				i++ ;
			}
		}
		}
	}
}
//------------------do prime factor actions---------------------------------
void do_primefactor()
{
	key_t key = ftok(".", 'd') ;
	check_key(key) ;
	if((prime_shm = shmget(key,101*sizeof(int), IPC_CREAT | IPC_EXCL | 0600)) == -1) {
		perror("shmget failed: ") ;
		exit(EXIT_FAILURE) ;
	}
	prime_ptr = (int *) shmat(prime_shm, NULL, 0) ;
	check_ptr(prime_ptr) ;
	signal(SIGUSR1, catch_sig1) ;

	while(1)
	{
		pause();
		primeFactors(prime_ptr[0], prime_ptr) ;
		kill(((pid_t*)shmat(shm_id,NULL,0))[2] , SIGUSR2) ;
	}
}
//--------------do gcd actions------------------------------------
void do_gcd()
{
	key_t key = ftok(".", 'g') ;
	check_key(key) ;
	if((gcd_shm = shmget(key,3*sizeof(int), IPC_CREAT | IPC_EXCL | 0600)) == -1) {
		perror("shmget failed: ") ;
		exit(EXIT_FAILURE) ;
	}
	gcd_ptr = (int *) shmat(gcd_shm, NULL, 0) ;
	check_ptr(gcd_ptr) ;
	signal(SIGUSR1, catch_sig1) ;

	while(1)
	{
		pause();
		gcd_ptr[2] = gcd(gcd_ptr[0], gcd_ptr[1]) ;
		kill(((pid_t*)shmat(shm_id,NULL,0))[2] , SIGUSR2) ;
	}
}
//------------checking the ftok key-------------------------
void check_key(key_t key)
{
	if(key == -1)
	{
		perror("ftok failed : ") ;
		exit(EXIT_FAILURE) ;
	}
}
//-------------gcd operation--------------------------------
int gcd(int a, int b)
{
	int temp;
	while (b != 0)
	{
		temp = a % b;

		a = b;
		b = temp;
	}
	return a;
}
//--------------checking the fork-------------------------------------
void is_valid(pid_t status)
{
	if(status < 0)
	{
		perror("Cannot fork()");
		exit(EXIT_FAILURE) ;
	}
}
//-------------------checking shmat system call-------------------------
void check_ptr(int *ptr)
{
	if (!ptr) {
		perror("shmat failed: ") ;
		exit(EXIT_FAILURE) ;
	}
}
//---------------prime factors operation------------------------------------
void primeFactors(int n, int *arr)
{
	int i, j = 1;

	// Print the number of 2s that divide n
	while (n%2 == 0)
	{
		arr[j] = 2 ;
		j++ ;
		n = n/2;
	}

	// n must be odd at this point.  So we can skip
	// one element (Note i = i +2)
	for (i = 3; i <= n/2; i = i+2)
	{
		// While i divides n, print i and divide n
		while (n%i == 0 )
		{
			arr[j] = i ;
			j++ ;
			n /= i;
		}
	}

	// This condition is to handle the case when n
	// is a prime number greater than 2
	if (n > 2)
	{
		arr[j] = n ;
		j++ ;
	}

	arr[j]=-1;
}
//--------------continue by user choice to use the servers-----------------------
void signal_handler(int num)
{
	if(pid_arr[1] == getpid()){
		shmdt(prime_ptr) ;
		if(shmctl(prime_shm, IPC_RMID, NULL) == -1){
			perror("prime failed");
			exit(EXIT_FAILURE);
		}
	}

	else if(pid_arr[0] == getpid()){

		shmdt(gcd_ptr) ;
		if(shmctl(gcd_shm, IPC_RMID, NULL) == -1){
			perror("gcd failed");
			exit(EXIT_FAILURE);
		}
	}
	else if(getpid() == fatherpid){
		shmdt(pid_arr) ;
		if(shmctl(shm_id, IPC_RMID, NULL) == -1){
			perror("shmctl failed");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS) ;
}
