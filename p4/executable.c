#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "matrix.h"
#include <assert.h>

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
double calculate_right_side(double *row, double right_side_value, double *x_values, int current_column, int size){
	int i = size;
	for (i; i >= 0; i--){
		if (current_column != i){
			right_side_value -= (row[i] * x_values[i]);
		}
	}
	return (right_side_value / row[current_column]);

}
double * back_substitution(Matrix **matrix, Vector **vector){
	int i = (*matrix)->size-1;
	double * x_values = calloc(sizeof(double), (*matrix)->size);
	for (i; i >= 0; i--){
		x_values[i] = 1;
		double new_x = calculate_right_side((*matrix)->values[i], (*vector)->values[i], x_values, i, (*matrix)->size-1);
		x_values[i] = new_x;
	}
	return x_values;
}
/*
 * Function:  get_input_from_user
 * Purpose:   Reads in the users input and summarizes how to run program
              if the user fails to pass the correct number of arguments
 * In args:   argc, argv
 * Out args:  n: number of rows and columns
              thread_count: number of threads to use
 */
double * execute_gaussian_elimination(Matrix ** matrix, Vector ** vector){
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
	return (back_substitution(&(*matrix), &(*vector)));
}

void get_input_from_user(int argc, char * argv[], int * n, int * thread_count){
	if (argc < 3){
		fprintf(stderr, "Must run '%s' with following arguments: <thread count> <n> <g|i>\n", argv[0]);
		exit(0);
	}
	(*n) = atoi(argv[1]);
    (*thread_count) = atoi(argv[2]);
}
double cross_product(double * matrix_row_values, double * x_values, int size){
	double result = 0;
	int i =0;
	for (i; i < size; i++){
	    printf("%f matrix_row_values[i] ... %f x_values[i]\n", matrix_row_values[i], x_values[i]);
		result += (matrix_row_values[i] * x_values[i]);
	}
	printf("%f .. \n", result);
	return result;
}
Vector * multiply_matrix_by_x_vector(Matrix * matrix, Vector * x_values, int n){
	Vector * result = create_vector(x_values->size, 0);
	int i = 0;
	for (i; i < x_values->size; i++){
		assert(matrix->size == x_values->size);
		result->values[i] = cross_product(matrix->values[i], x_values->values, n);
	}
	return result;
}
int main(int argc, char * argv[]){
	int n, thread_count;
	get_input_from_user(argc, argv, &n, &thread_count);
	srand48(time(NULL));
	Vector * vector = create_vector(n, 1);
	Matrix * matrix = create_matrix(n, 1);
	Vector * vector_original = copy_vector(vector);
	Matrix * matrix_original = copy_matrix(matrix);
	Vector * x_values = create_vector(n, 0);
	// print_matrix(matrix);
	// puts("\n\n\n");
	// print_vector(vector);
	// puts("BEFORE\n\n\n");
	x_values->values = execute_gaussian_elimination(&matrix, &vector);
	print_matrix(matrix);
	puts("\n\n\n");
	print_vector(vector);
	puts("\n\n\n");
	print_vector(x_values);
	puts("\n\n\n");
	Vector * resulting_vector = multiply_matrix_by_x_vector(matrix_original, x_values, n);
	print_vector(resulting_vector);
	puts("that was multiply vector\n\n\n");
	resulting_vector = subtract_vectors(resulting_vector, vector_original);
	print_vector(resulting_vector);
	puts("that was  subtract vector\n\n\n");
	printf("%f\n", l2_norm(resulting_vector));
	return 0;
}