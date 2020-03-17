 /**************************
 * ex4b2: Prime Factor Server
 * ============================================================================
 * The factor server makes a fifo and then runs at background,
 * waits for input from client, calculate the prime factors by the algo
 * and send the result back to client 	
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc -Wall ex4b2.c -o ex4b2
 * Run: ./ex4b2
 * ============================================================================
 * Input: got it from the client
 * ============================================================================
 * Output: send the result back to client
 * ============================================================================
 */

//---------------Include------------------------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <signal.h>

//------------prototypes-------------------
int primeFactors(int n, FILE *fdw, int arr[]);
void signak_hadler(int i);
void open_namedpipe() ;

//----------main section------------------
int main()
{
	signal(SIGINT, signak_hadler);

	FILE *fdr, *fdw ;
	open_namedpipe() ;

	int n1, status, i ;

	int arr[100] = {0};
	while(1)
	{
		if(!(fdr = fopen("my_pipe3","r")))
		{
			perror("Cannot open fifo file for r");
			exit(EXIT_FAILURE);
		}

		status = fscanf(fdr, "%d",&n1);
		fclose(fdr) ;

		if(status > 0)
		{
				if(!(fdw = fopen("my_pipe4","w")))
				{
					perror("Cannot open fifo file for w");
					exit(EXIT_FAILURE);
				}

			arr[0] = primeFactors(n1,fdw, arr);
			for(i = 1; i < arr[0] ; i++)
			{
				fprintf(fdw,"%d " ,arr[i]);
			}
			fprintf(fdw,"\n");
			fflush(fdw);
			fclose(fdw);
		}
	}

	return EXIT_SUCCESS;
}


// -----A function to print all prime factors of a given number n--------
int primeFactors(int n, FILE *fdw, int arr[])
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
	return j ; //returns size

}
//-----------signak handler------------------------------
void signak_hadler(int i)
{
	system("rm my_pipe3");
	system("rm my_pipe4");
	exit(EXIT_SUCCESS);
}

//-------------opening namepipe------------------------------
void open_namedpipe()
{
	if(mkfifo("my_pipe3", S_IFIFO | 0644 ) == -1)
	{
		perror("cannot create fifo file");
		exit(EXIT_FAILURE);
	}

	if(mkfifo("my_pipe4", S_IFIFO | 0644 ) == -1)
	{
		perror("cannot create fifo file");
		exit(EXIT_FAILURE);
	}
}

