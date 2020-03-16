/******************************************************************************
 * Exr. #1: * This program creates 5 children, that each does the Towers Of Hanoi
 * algorithm. child number i=0 does the algorithm with 10 rings, child num i=1
 * does the algorithm with 20 rings, i=2 with 25, i=3 with 30 and i=4 with 35
 * rings.
 * the program will pring the time took each child to get the job done.
 * =============================================================================
 * Written by  	: Tomer Fisher & Shahar Aviv
 * login	: tomerfi / shaharav
 * Year	   		: B
 * =============================================================================
 * Input: An input file.
 * -----------------------------------------------------------------------------
 * Output: the time to each child to do the Hanoi job
 ******************************************************************************/
// ================================ Include ================================ //
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
//-------------------------------------------------
void hanoi(int size, char src, char dest, char aux, FILE *fdr) ;
void do_child(int child, FILE *fdr) ;
void solve_child(int n, FILE *fdr) ;
void is_valid(pid_t status) ;
void check_file(FILE *fdr) ;
//-------------------------------------------------
int main()
{
	FILE *fdr = fopen("/dev/null", "w") ;
	check_file(fdr) ;

	pid_t status ;
	int i ;
	srand((unsigned) time(NULL)) ;

	for(i = 0 ; i < 5 ;i++)
	{
		status = fork() ;
		is_valid(status) ;

		if(status == 0)
		{
			do_child(i,fdr) ;
			return(EXIT_SUCCESS) ;
		}
	}

	fclose(fdr) ;
	return EXIT_SUCCESS ;
}
//-------------------------------------------------
void do_child(int child, FILE *fdr)
{
	switch (child)
	{
	case 0:
		solve_child(10, fdr) ;
		break ;
	case 1:
		solve_child(20, fdr) ;
		break ;
	case 2:
		solve_child(25, fdr) ;
		break ;
	case 3:
		solve_child(30, fdr) ;
		break ;
	case 4:
		solve_child(35, fdr) ;
		break ;
	}
}
//-------------------------------------------------
void solve_child(int n, FILE *fdr)
{
	time_t start, end ;

	start = time(NULL) ;
	hanoi(n, 'A', 'B', 'C', fdr) ;
	end = time(NULL) ;

	printf("%d = %d \n", n, (int) end - (int) start) ;
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
//-------------------------------------------------
void is_valid(pid_t status)
{
	if(status < 0)
	{
		perror("Cannot fork()");
		exit(EXIT_FAILURE) ;
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
