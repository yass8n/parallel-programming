#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "matrix.h"
#include <complex.h>
void swap_matrix_row(Matrix **matrix, int initial_row, int swap_row){
	double * initial_row_pointer = (*matrix)->values[initial_row];
	(*matrix)->values[initial_row] = (*matrix)->values[swap_row];
	(*matrix)->values[swap_row] = initial_row_pointer;
}
void swap_vector_row(Vector **vector, int initial_row, int swap_row){
    double initial_value = (*vector)->values[initial_row];
	(*vector)->values[initial_row] = (*vector)->values[swap_row];
	(*vector)->values[swap_row] = initial_value;
}
void forward_elimination(Matrix **matrix, Vector **vector, int row){
	int static_column = row;
	for (++row; row < (*matrix)->size; row++){
		double multiplier = (*matrix)->values[row][static_column] / (*matrix)->values[static_column][static_column];
		int new_column = static_column;
		double * temp_row = calloc(sizeof(double), (*matrix)->size);
		for (new_column; new_column < (*matrix)->size; new_column++){
			temp_row[new_column] = (*matrix)->values[static_column][new_column];
			temp_row[new_column] *= multiplier;
			(*matrix)->values[row][new_column] = (*matrix)->values[row][new_column] - temp_row[new_column];
		}
		double temp_vect_value = (*vector)->values[static_column];
		temp_vect_value *= multiplier;
		(*vector)->values[static_column+1] = (*vector)->values[static_column+1] - temp_vect_value;
	}
}
/*
 * Function:  get_input_from_user
 * Purpose:   Reads in the users input and summarizes how to run program
              if the user fails to pass the correct number of arguments
 * In args:   argc, argv
 * Out args:  n: number of rows and columns
              thread_count: number of threads to use
 */
void execute_gaussian_elimination(Matrix ** matrix, Vector ** vector){
	int i = 0, j = 0, k = 0; 
	for (i,k; i < (*matrix)->size; i ++, k++){
		double largest = cabs((*matrix)->values[i][k]);
		int row = i;
		for (j = k+1; j < (*matrix)->size; j ++){
			if ( cabs((*matrix)->values[j][k]) > largest ){
				largest = cabs((*matrix)->values[j][k]);
				row = j;
			}
		}
		swap_matrix_row(&(*matrix), i, row);
		swap_vector_row(&(*vector), i, row);
		forward_elimination(&(*matrix), &(*vector), i);
	}
}

void get_input_from_user(int argc, char * argv[], int * n, int * thread_count){
	if (argc < 3){
		fprintf(stderr, "Must run '%s' with following arguments: <thread count> <n> <g|i>\n", argv[0]);
		exit(0);
	}
	(*n) = atoi(argv[1]);
    (*thread_count) = atoi(argv[2]);
}
int main(int argc, char * argv[]){
	int n, thread_count;
	get_input_from_user(argc, argv, &n, &thread_count);
	srand48(time(NULL));
	Vector * vector = create_vector(n);
	Matrix * matrix = create_matrix(n);
	print_matrix(matrix);
	puts("\n\n\n");
	print_vector(vector);
	puts("BEFORE\n\n\n");
	execute_gaussian_elimination(&matrix, &vector);
	print_matrix(matrix);
	puts("\n\n\n");
	print_vector(vector);
	return 0;
}