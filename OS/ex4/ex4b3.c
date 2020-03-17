 /**************************
 * ex4b3: the client
 * ============================================================================
 *
 * The client program gets arguments then go to the relevant server by reading 1 
 * to prime factors and 0 to gcd. 
 * the client gets the relevant result from the relevant server, print it and ends
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc -Wall ex4b3.c -o ex4b3
 * Run: ./ex4b3
 * ============================================================================
 * Input: got it from args 
 * ============================================================================
 * Output: the relevant result from the relevant server
 * ============================================================================
 */

//---------------Include------------------------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

//----------main section------------------
int main(int argc ,char *argv[])
{

	int num ;
	FILE *fdr,*fdw;
	int n1,n2;

	if(argc < 3)
	{
		puts("not enough arguments");
		exit(EXIT_FAILURE);
	}

	if(atoi(argv[1]) == 0)
	{
		if(atoi(argv[3])<= 0 || atoi(argv[2])<= 0 )
		{

			printf("Argument numbers are unvalid\n");
			exit(EXIT_FAILURE);
		}
	}
	if(atoi(argv[1]) == 1)
	{


		if(atoi(argv[2])<= 1 )
		{
			printf("Argument numbers are unvalid\n");
			exit(EXIT_FAILURE);
		}
	}

	if(atoi(argv[1]) == 1)
	{
		n1 = atoi(argv[2]);
		if(!(fdw = fopen("my_pipe3","w")))
				{
					perror("Cannot open fifo file for w");
					exit(EXIT_FAILURE);
				}
		fprintf(fdw,"%d\n" ,n1);
		fflush(fdw);
		if(!(fdr = fopen("my_pipe4","r")))
				{
					perror("Cannot open fifo file for r");
					exit(EXIT_FAILURE);

				}

		while((fscanf(fdr,"%d",&num))!= EOF)
		{
			printf("%d ", num);
		}
		puts("");
	}
	else if(atoi(argv[1]) == 0)
	{
		n1 = atoi(argv[2]);
		n2 = atoi(argv[3]);
		if(!(fdw = fopen("my_pipe1","w")))
		{
			perror("Cannot open fifo file for w");
			exit(EXIT_FAILURE);
		}
		

		printf("Sending %d %d\n",n1,n2);
		fprintf(fdw,"%d %d\n" ,n1,n2);
		fflush(fdw);
		if(!(fdr = fopen("my_pipe2","r")))
		{
			perror("Cannot open fifo file for r");
			exit(EXIT_FAILURE);
		}
		puts("opened read");
		fscanf(fdr,"%d",&n1);
		printf("%d \n",n1);
	}
	fclose(fdr);
	fclose(fdw);

	return EXIT_SUCCESS;
}

