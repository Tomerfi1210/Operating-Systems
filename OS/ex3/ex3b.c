/* File: ex3b.c
	Sorting with fork
============================================================================
The father of this Program initializes array with random numbers from zero
to a max value then splits the array into number of chldren pieces.
The father then send each piece of the array to each child to be sorted
using bubble sort.
when each child finishes it then writes the sorted array into a pipe.
when all children are done the father reads from this pipe all the pieces
into one big array.
it then merges this big array.
printd the min and max
Written by:Tomer Fisher & Shahar aviv
Login: tomerfi	shaharav
Input: None

Output: Min and max values of fully sorted array(first,last cell)
 * */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <wait.h>
//----------defines------------------
#define SIZE 500
#define MAX 1001
#define NUM_OF_CHILD 5
#define STOP SIZE/NUM_OF_CHILD
//----------prototypes-------------------
void do_child(int arr[], int pipe_arr[]);
void do_dad(int pipe_arr[]);
void swap(int *xp, int *yp);
void bubbleSort(int arr[], int n);
void merge(int arr[], int left, int middle, int right);
//----------main section------------------

int main()
{

	pid_t status;
	int arr[SIZE];
	int i, c;
	int pipe_arr[2];

	//creating pipe
	if (pipe(pipe_arr) ==-1)
	{
		perror("cannot for\n");
		exit(EXIT_FAILURE);
	}
	srand(17); // starting rand action

	for (i = 0; i < SIZE; i++)
	{
		arr[i] = (rand() % MAX);
	}

	for (c = 0; c < NUM_OF_CHILD; c++)
	{
		status = fork();
		if (status < 0)
		{
			fputs("error in fork", stderr);
			exit(EXIT_FAILURE);
		}
		if (status == 0)
		{
			//sending part of array for child to sort
			do_child(&arr[c*STOP], pipe_arr);
			exit(EXIT_SUCCESS);
		}
	}

	//waiting for all children to finish sorting there part
	for(i=0; i<NUM_OF_CHILD; i++)
		wait(NULL);

	//dad reads the whole array, then merges, prints min and max
	do_dad(pipe_arr);


	return 0;
}
//-------------------------------------------
void do_dad(int pipe_arr[])
{
	int f;
	int middle, end;
	int final_arr[SIZE];
	//closing side of input
	if (close(pipe_arr[1]) == -1)
	{
		perror("cannot close\n");
		exit(EXIT_FAILURE);
	}
	//reading all the parts into one array
	if (read(pipe_arr[0], final_arr, SIZE*sizeof(int)) == -1)
	{
		perror("cannot read\n");
		exit (EXIT_FAILURE);
	}
	//running through parts and merging them
	for (f=0; f<NUM_OF_CHILD-1; f++)
	{
		middle = (f+1) * (STOP)-1;
		end    = (f+2) * (STOP)-1;
		merge(final_arr, 0, middle, end);
	}

	//printing out min and max values
	printf("The min value in arry is: %d\n", final_arr[0]);
	printf("The max value in arry is: %d\n", final_arr[SIZE-1]);

	if(close(pipe_arr[0]) == -1)
	{
		perror("cannot close\n");
		exit(EXIT_FAILURE);
	}
}
//-------------------------------------------
//gets part of full array and sorts
void do_child(int arr[], int pipe_arr[])
{

	if(close(pipe_arr[0])==-1)
	{
		perror("Cannot Close");
		exit(EXIT_FAILURE);
	}
	//sorts my part
	bubbleSort(arr, STOP);
	if(write(pipe_arr[1], arr, STOP*sizeof(int))==-1)
	{
		perror("Cannot Write");
		exit(EXIT_FAILURE);
	}
	if(close(pipe_arr[1])==-1)
	{
		perror("Cannot Close");
		exit(EXIT_FAILURE);
	}
}

//----------------------------------------------
void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
	int i, j;
	for (i = 0; i < n-1; i++)

		// Last i elements are already in place
		for (j = 0; j < n-i-1; j++)
			if (arr[j] > arr[j+1])
				swap(&arr[j], &arr[j+1]);
}

//-------------------------------------------------------
//merge function merges two arrays
void merge(int arr[], int left, int middle, int right)
{
	int i, j, k;
	int n1 = middle - left + 1;
	int n2 =  right - middle;

	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[left + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[middle + 1+ j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = left; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
       are any */
	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
       are any */
	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
}
