 /**************************
 * ex4b1: GCD Server
 * ============================================================================
 * Thee gcd server makes named pipe and run. waits for input from the client
 * calculate the gcd by gcd algo and send the result to client
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shharav, tomerfi
 *
 * Compile: gcc -Wall ex4b1.c -o ex4b1
 * Run: ./ex4b1
 * ============================================================================
 * Input: got it from client
 * ============================================================================
 * Output: writes the result back to client
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
int gcd(int n1,int n2) ;
void signal_catch(int i) ;
void open_namedpipe() ;

//----------main section------------------
int main()
{
	signal(SIGINT, signal_catch);

	FILE *fdr, *fdw ;
	open_namedpipe() ;

	int n1,n2,result, status ;

	while(1)
	{
		if(!(fdr = fopen("my_pipe1","r")))
		{
			perror("Cannot open fifo file for r");
			exit(EXIT_FAILURE);
		}
		status = fscanf(fdr, "%d %d",&n1,&n2);
		fclose(fdr);

		if(status > 0)
		{
			result = gcd(n1,n2);
			if(!(fdw = fopen("my_pipe2","w")))
			{
				perror("Cannot open fifo file for w");
				exit(EXIT_FAILURE);
			}

			fprintf(fdw,"%d\n" ,result);
			fflush(fdw);
			fclose(fdw);
		}
	}
	return EXIT_SUCCESS;
}
//--------calculat gcd--------------------------------
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
//------------signal handler----------------------------
void signal_catch(int i)
{
	system("rm my_pipe1");
	system("rm my_pipe2");
	exit(EXIT_SUCCESS);
}
//---------------opening name pipe------------------------------
void open_namedpipe()
{
	if(mkfifo("my_pipe1", S_IFIFO | 0644 ) == -1)
	{
		perror("cannot create fifo file");
		exit(EXIT_FAILURE);
	}

	if(mkfifo("my_pipe2", S_IFIFO | 0644 ) == -1)
	{
		perror("cannot create fifo file");
		exit(EXIT_FAILURE);
	}
}
