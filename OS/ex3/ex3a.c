/*
 * ex3a.c
 *
 *  Created on: Nov 23, 2018
 *      Author: tomerfi
 */
//===============================================================================
/*
 		(*) Tomer Fisher - 205364151 - tomerfi
 		(*) Shahar Aviv - 203489307 - shaharav
===============================================================================
Ex3a: Pipe part a
===============================================================================
In program the father gives birth to five children of each type, meaning
that each child solves the problem on a tower of a given size, when the sizes
will be: 5, 10, 15, 20, 25.
The children will be sent to Dad through the tube and the tower size they were
asked to solve, and the time
The running they needed to solve the problem.
The father will print the minimum runtime, maximum and average time required
to solve the problem on
Tower size n.
The children will continue to write their information to the standard output,
but the information will reach the father
Through a tube. The children work concurrently during the program.
*/
//------------------INCLUDES----------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <limits.h>

//------------------Define and Consts---------------------------
#define TOWERS 5
#define CHILDS 5
////------------------Prototype--------------------------------
void check_pipe(int pipes[]) ;
void check(pid_t status) ;
void hanoi(int size, char src, char dest, char aux, FILE *fdr) ;
void check_file(FILE *fdr) ;
void do_child(int tower, int pipe_descs[], FILE *fdr) ;
void do_dad(int pipe_descs[], int max[], int min[] , int arr[]) ;
//-------------------Main----------------------------------------
int main()
{
	pid_t status ;
	int pipe_descs[2] ;

	FILE *fdr = fopen("/dev/null", "w") ;
	check_file(fdr);

	srand((unsigned) time(NULL)) ;
	int i, j, k, z, t, ring = 5 , arrMax[5] = {-1, -1, -1, -1, -1},
			arrMin[5] = {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX}, arrAverage[5] = {0} ;

	check_pipe(pipe_descs) ;
	for(i = 0 ; i < TOWERS ; i++)
	{
		for(j = 0 ; j < CHILDS ; j++)
		{
			status = fork() ;
			check(status) ;

			if(status == 0)
			{
				do_child(ring, pipe_descs, fdr) ;
				return EXIT_SUCCESS ;
			}
		}
		ring += 5;
	}
	for(k = 0 ; k < 25 ; k++)
		wait(NULL) ; //waiting for the last child to finish.


	for(i=0;i<5;i++) {
		//for each ring size i check their times
		for(z = 0 ; z < 5 ;z++){
			do_dad(pipe_descs, arrMax, arrMin, arrAverage) ;
		}

	}


	for(t = 0 ; t < TOWERS ; t++)
		printf("%d. max: %d, min: %d, avg: %f\n", t+1, arrMax[t], arrMin[t], (float)arrAverage[t]/5) ;

	fclose(fdr) ;
	return EXIT_SUCCESS ;
}
//---------------------------------------------------
void do_dad(int pipe_descs[], int max[], int min[], int arrAverage[])
{
	int buff[2] ;//to read from the pipe .
	close(pipe_descs[1]) ; // close for writing.
	dup2(pipe_descs[0], STDIN_FILENO) ;

	scanf("%d %d", &buff[0],&buff[1]) ;
	int run_time = buff[1] ;
	int rings = buff[0] ;
	int child ;

	switch(rings)
	{
	case 5:
		child = 0;
		break ;
	case 10:
		child = 1 ;
		break ;
	case 15:
		child = 2 ;
		break ;
	case 20:
		child = 3 ;
		break;
	case 25:
		child = 4 ;
		break ;
	}

	if(run_time > max[child])
		max[child] = run_time ;
	if(run_time < min[child])
		min[child] = run_time ;
	arrAverage[child] += run_time ;

	close(pipe_descs[0]);
}
//---------------------------------------------------
void do_child(int tower, int pipe_descs[], FILE *fdr)
{
	time_t start,
	end ;

	close(pipe_descs[0]); //close for reading.
	dup2(pipe_descs[1], STDOUT_FILENO);

	start = time(NULL) ;
	hanoi(tower, 'A', 'B', 'C', fdr) ;
	end = time(NULL) ;

	printf("%d %d\n", tower, (int)end - (int)start) ;
	close(pipe_descs[1]); // close for writing.
}
//-------------------------------------------------
void hanoi(int size, char src, char dest, char aux, FILE *fdr)
{
	if(size == 1)
	{
		fprintf(fdr, "\n Move disk 1 from rod %c to rod %c ", src, dest) ;
		return ;
	}
	hanoi(size - 1, src, aux, dest, fdr);
	fprintf(fdr, "\n Move disk %d from %c to rod %c ", size, src, dest) ;
	hanoi(size - 1, aux, dest, src, fdr) ;
}
//----------------------------------------
void check_pipe(int pipes[])
{
	if (pipe(pipes) == -1)
	{
		perror("cannot open pipe") ;
		exit(EXIT_FAILURE) ;
	}
}
//------------------------------------------
void check(pid_t status)
{
	if (status < 0)
	{
		perror("Cannot fork!\n");
		exit(EXIT_FAILURE);
	}
}
//-----------------------------------------------------
void check_file(FILE *fdr)
{
	// checking if file opened successfully
	if (fdr == NULL) {
		fputs("Cannot open file \n", stderr);
		exit(EXIT_FAILURE);
	}
}
