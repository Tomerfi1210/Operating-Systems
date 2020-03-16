/******************************************************************************
 * ex8a: sort and merge arrays using threads
 * ============================================================================
 * The program creates 5 threads. Every one of them allocates an array and fills
 * it with numbers randomally. Every thread sorts it own array and
 * send it back to the main process. In the end the main process sort and
 * merges them.
 * array and prints the sorted array.
 *
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc ex8a.c -o thread -lpthread
 * Run: ./thread
 * ============================================================================
 * Input: None.
 * ============================================================================
 * Output: Mmerged array.
 * ============================================================================
 * */

//--------------uncludes------------
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
//----------defines------------------
pthread_key_t key;
#define ARR_SIZE 10
//----------prototypes--------------------
void* thread_func(void* args);
void my_sort();
void copy(int *dest,const int* src,const int n);
void merge(int arr[], int l, int m, int r);
void sendMerge(int arr[],int size);
void mergeSort(int arr[], int l, int r);
void print(const int* arr,const int size);
void freemem(int *arr[],int n);
void swap(int *a,int*b);
//------------main section-------------

int main()
{

	pthread_t thread_data[5];
	int status;
	int i, rc;
	int *results[5]; //result of 5 threads
	int totalArray[ARR_SIZE*5];
	srand(17);

	rc = pthread_key_create(&key, NULL); //creation of the key
	if(rc)
	{
		fputs("pthread_key_create failed", stderr);
		exit(EXIT_FAILURE);
	}

	for (i=0; i<5; i++) //creation of 5 threads
	{

		status = pthread_create(&thread_data[i], NULL, thread_func, NULL);
		if (status != 0)
		{
			fputs("pthread create failed in main", stderr);
			exit(EXIT_FAILURE);
		}

	}

	for (i=0; i<5; i++) //waiting of threads
	{
		status = pthread_join(thread_data[i],(void**)&results[i]);
		if (status != 0)
		{
			fputs("pthread create failed in main", stderr);
			exit(EXIT_FAILURE);
		}
		copy(&totalArray[i*ARR_SIZE],results[i],10);

	}


	printf("Merged:\n");
	sendMerge(totalArray,50);
	print(totalArray,50);


	freemem(results,5);


	return EXIT_SUCCESS;

}
//----------send merge function-------------------------------------
void sendMerge(int arr[],int size)
{
	//merging parts of array together
	int i;
	for(i=0;i<5-1;i++)
	{
		int mid = (i+1)*(ARR_SIZE)-1;
		int end = mid+ARR_SIZE;
		merge(arr, 0, mid, end);  //merging 2 parts of the array
	}

}
//-------------thread actions function-------------------------------
void* thread_func(void* args)
{
	int rc;
	int i;


	int* arr = (int*) malloc(ARR_SIZE*sizeof(int)); //allocation of array
	if(arr==NULL) //check allocation
	{
		puts("malloc failed");
		exit(EXIT_FAILURE);
	}

	rc = pthread_setspecific(key, arr); //set specific with the key
	if(rc)
	{
		fputs("pthread_setspecific failed", stderr);
		exit(EXIT_FAILURE);
	}

	for (i=0; i<ARR_SIZE; i++) //random numbers in the array
	{
		arr[i] = (rand() % 1000 + 1);
	}

	my_sort(); //calling to sorting algorithem

	pthread_exit((void*)arr);

}
//-------------my sort function---------------------------------
void my_sort()
{
	int *arr = (int*)pthread_getspecific(key);

	mergeSort(arr,0,9);
}
//-------------merge arrays-function----------------
void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 =  r - m;

	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1+ j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
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
//--------------print array function-----------------------
void print(const int* arr,const int size)
{
	int i;
	for(i=0;i<size;i++)
		printf("%d ",arr[i]);
	puts("");
}
//-----------copy function---------------------------------
void copy(int *dest,const int* src,const int n)
{
	int i;
	for(i=0;i<n;i++)
		dest[i] = src[i];
}
//---------merge sort algorithem--------------------------
void mergeSort(int arr[], int l, int r)
{
	if (l < r)
	{
		// Same as (l+r)/2, but avoids overflow
		// large l and h
		int m = l+(r-l)/2;

		// Sort first and second halves
		mergeSort(arr, l, m);
		mergeSort(arr, m+1, r);

		merge(arr, l, m, r);
	}
}

//--------------swap function------------------------------
void swap(int *a,int*b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
//---------------free memory function----------------------
void freemem(int *arr[],int n)
{
	int i;
	for(i=0;i<n;i++)
		free(arr[i]);
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
