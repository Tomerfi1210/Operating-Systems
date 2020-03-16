/******************************************************************************
 * Exr. #0: This program get through the argument vector name of input file.
 *          The program read the input file and creates a 2d-dynamic array.
 * =============================================================================
 * Written by  	: Tomer Fisher & Shahar Aviv
 * login	   		: tomerfi / shaharav
 * Year	   		: B
 * =============================================================================
 * Input: An input file.
 * -----------------------------------------------------------------------------
 * Output: The row in the created dynamic array that all of it's elements are
 *         are greater respectively from the elements on the same column
 *         (if exist)
 ******************************************************************************/
// ================================ Include ================================ //

#include <stdio.h>
#include <stdlib.h>

// ================================ Structs ================================ //

typedef struct {
	int _num_of_lines;          //
	int ** _the_data;           //
	int * _lines_len;           //
}Data;
// ============================= Prototypes =============================== //
void make_data(FILE *fdr, Data* data);
void make_len(int* *lines_len, int* num_of_lines, int size, int index);
void free_all(Data * data);
void check_alloc_1d(int* array);
void check_alloc_2d(int** array);
void check_alloc_3d(Data *data);
int  find_row(Data * data);
void check_file(FILE *fdr);
void creat_and_insert(Data *data , FILE *fdr , int *row_size , int *curr_row) ;
// ================================= Main ================================== //
int main(int argc, char* argv[])
{
	FILE* fdr = fopen(argv[1], "r");
	check_file(fdr) ;
;      // File descriptor for input file

	// there is also an input file name
	if (argc != 2){
		fprintf(stderr,"%s Usage: <input file> \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	
	Data* data = (Data*) malloc(sizeof(Data) * 1);
	check_alloc_3d(data);
	make_data(fdr, data);

	printf("row = %d \n",find_row(data));

	//free all memory used by program and closed used files
	free_all(data);
	free(data);
	fclose(fdr);

	return EXIT_SUCCESS;
}

// =============================== Functions =============================== //

// This function gets a File of an input file and the struct of
// type "Data".
// Update the struct members, mainly build a 2d-dynamic array using the
// file's content.
void make_data(FILE *fdr, Data* data)
{
	int **lines_len = &data->_lines_len,
			*num_of_lines = &(data->_num_of_lines);

	int row_size,       // keeps the size of the current row we read
	curr_row = 0,       // keeps the number of the current row we read
	allocate_row = 0;   // keeps the size of the rows we allocated

	// initilize the struct members and memory
	*num_of_lines = 0;
	data->_the_data = malloc(sizeof(int**) * 0);
	check_alloc_2d(data->_the_data);
	*lines_len = malloc(sizeof(int*) * 0);
	check_alloc_1d(*lines_len);

	fscanf(fdr, "%d", &row_size);       // try to read a number

	data->_the_data = realloc(data->_the_data,  1 * sizeof(int**) );
	allocate_row++;
	check_alloc_2d(data->_the_data);

	while (!feof(fdr))
	{
		if (allocate_row == *num_of_lines)
		{
			data->_the_data = realloc(data->_the_data, (curr_row* 2) * sizeof(int**) );
			allocate_row *= 2;
			check_alloc_2d(data->_the_data);
		}
		make_len(lines_len, &(*num_of_lines),row_size, curr_row);
		creat_and_insert(data , fdr , &row_size , &curr_row) ;
	}
}
//------------------------------------------------------------------------------------------
void creat_and_insert(Data *data , FILE *fdr , int *row_size , int *curr_row)
{
	int col ;
	(data->_the_data)[*curr_row] = (int*) malloc(sizeof(int) * (*row_size));
	check_alloc_2d(data->_the_data);

	// inserts the data of the line into the 2d-dynamic array we created
	for (col = 0; col < *row_size; col++)
		fscanf(fdr, "%d", &(data->_the_data)[*curr_row][col]);

	(*curr_row)++;                  //we used a row
	fscanf(fdr, "%d", row_size);
}
//--------------------------------------------------------------------------

// this functions gets a struct from type "Data"
// The function scans the the 2d-dynamic array member of the struct, and
// search if there is a row that all of its elements are greater than the
// other elemnts in the respctive column. if so, return the number of the
// row , otherwise - return (-1)
int find_row(Data * data)
{
	int curr_row,               // index of the current row we check
	curr_col,               // index of the current column we check
	curr_cell,              // the cell we checked
	row;                    // index for running on rows

	for (curr_row = 0; curr_row < data->_num_of_lines ; ++curr_row){
		for (curr_col = 0; curr_col < data->_lines_len[curr_row]; ++curr_col)
		{
			curr_cell = data->_the_data[curr_row][curr_col];

			for (row = 0; row < data->_num_of_lines; ++row) {
				if (row == curr_row)
					continue;
				if (curr_col >= data->_lines_len[row])
					continue;
				if (data->_the_data[row][curr_col] > curr_cell)
					break;
			}

			// if we didn't reach the end of the total number of rows, it mean
			// this row doesn't apply to the condition
			if (row < data->_num_of_lines)
				break;
		}
		// if we reached the end of the current row, it mean this row apply to
		// the condition
		if (curr_col == data->_lines_len[curr_row])
			return curr_row;

	}

	return -1;  // we failed to find appropriate row
}

//----------------------------------------------------------------------------/

// this function get a pointer of 1d-dynamic array
// Checks if the allocation succeed
void check_alloc_1d(int* array)
{
	if (array == NULL) {
		fputs("Cannot allocate memory", stderr);
		exit(EXIT_FAILURE);
	}
}
//----------------------------------------------------------------------------/
// The function gets a pointer to the lines-length 1d-array, pointer to thr
// num of rows ,the size and the index of the row we have to add its size
// The function update the 1d-array
void make_len(int **lines_len, int* num_of_lines, int size, int index)
{
	(*lines_len) = (int*) realloc(*lines_len,
			(*num_of_lines + 1) * sizeof(int));
	check_alloc_1d(*lines_len);

	(*lines_len)[index] = size; // update the row size in array
	++(*num_of_lines);          //update total size of rows
}
//----------------------------------------------------------------------------/

// this function get a pointer of 2d-dynamic array
// Checks if the allocation succeed
void check_alloc_2d(int** array)
{
	if (array == NULL) {
		fputs("Cannot allocate memory", stderr);
		exit(EXIT_FAILURE);
	}
}
//----------------------------------------------------------------------------/

// this functions gets a struct from type "Data"
// Frees all the memory the program used by this struct
void free_all(Data * data)
{
	int row;            // index for running on the 2d-dynamic array's rows
	for (row = 0; row < data->_num_of_lines; row++)
		free(data->_the_data[row]);

	free(data->_the_data);
	free(data->_lines_len);
}
//--------------------------------------------------------------
void check_file(FILE *fdr)
{
	// checking if file opened successfully
	if (fdr == NULL) {
		fputs("Cannot open file \n", stderr);
		exit(EXIT_FAILURE);
	}
}
//----------------------------------------------------------------
void check_alloc_3d(Data *data)
{
	if (data == NULL) {
			exit(EXIT_FAILURE);
		}
}
