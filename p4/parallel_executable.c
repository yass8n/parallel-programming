#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "matrix.h"
#include <assert.h>
#include <omp.h>

int thread_count;
void forward_elimination(Matrix **matrix, Vector **vector, int row){
	int static_column = row;
	#pragma omp parallel for num_threads(thread_count) shared(matrix, vector, static_column)\
	 private(row) schedule(dynamic,1)
	for (row = static_column + 1; row < (*matrix)->size; row++){
		printf("%d my thread number \n ", omp_get_thread_num());
		double multiplier = (*matrix)->values[row][static_column] / (*matrix)->values[static_column][static_column];
	    int new_column = static_column;
		double * temp_matrix_row = calloc(sizeof(double), (*matrix)->size);
		for (new_column = new_column; new_column < (*matrix)->size; new_column++){
				temp_matrix_row[new_column] = (*matrix)->values[static_column][new_column];
				temp_matrix_row[new_column] *= multiplier;
				(*matrix)->values[row][new_column] = (*matrix)->values[row][new_column] - temp_matrix_row[new_column];
		}
		double temp_vect_value = (*vector)->values[static_column];
		temp_vect_value *= multiplier;
	    (*vector)->values[row] = (*vector)->values[row] - temp_vect_value;
	}
}
double calculate_right_side(double *row, double right_side_value, double *x_values, int current_column, int size){
	int i = size;
	#pragma omp parallel for num_threads(thread_count) shared(row, x_values, current_column, size)\
	private(i) reduction(-:right_side_value) 
	for (i = size; i >= 0; i--){
		if (current_column != i){
			right_side_value -= (row[i] * x_values[i]);
		}
	}
	return (right_side_value / row[current_column]);

}
double * back_substitution(Matrix **matrix, Vector **vector){
	int i = (*matrix)->size-1;
	double * x_values = calloc(sizeof(double), (*matrix)->size);
	#pragma omp parallel for num_threads(thread_count) shared(matrix, vector, x_values)\
	private(i) schedule(dynamic, 1)
	for (i = (*matrix)->size-1; i >= 0; i--){
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
		int i = 0, j = 0; 
		for (i=0; i < (*matrix)->size; i ++){
			double largest = cabs((*matrix)->values[i][i]);
			int row = i;
			for (j = i+1; j < (*matrix)->size; j ++){
				if ( cabs((*matrix)->values[j][i]) > largest ){
					largest = cabs((*matrix)->values[j][i]);
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
	int i;
	#pragma omp parallel for num_threads(thread_count) shared(matrix_row_values, x_values)\
	private(i) reduction(+:result) 
	for (i = 0; i < size; i++){
		result += (matrix_row_values[i] * x_values[i]);
	}
	return result;
}
Vector * multiply_matrix_by_x_vector(Matrix * matrix, Vector * x_values, int n){
	Vector * result = create_vector(x_values->size, 0);
	int i;
	#pragma omp parallel for num_threads(thread_count) shared(matrix, x_values, result)\
	 private(i) schedule(dynamic,1)
	for (i = 0; i < x_values->size; i++){
		assert(matrix->size == x_values->size);
		result->values[i] = cross_product(matrix->values[i], x_values->values, n);
	}
	#pragma omp barrier
	return result;
}
double get_current_time()
{
    struct timeval t; 
    gettimeofday(&t, NULL);
    return t.tv_sec + (t.tv_usec / (double) 1000000.0);
}
int main(int argc, char * argv[]){
	int n;
	get_input_from_user(argc, argv, &n, &thread_count);
	// srand48(time(NULL));


	Vector * vector = create_vector(n, 1);
	Matrix * matrix = create_matrix(n, 1);
	Vector * vector_original = copy_vector(vector);
	Matrix * matrix_original = copy_matrix(matrix);
	Vector * x_values = create_vector(n, 0);


	double start_time = get_current_time();
		// print_matrix(matrix);
	// puts("original matrix\n\n\n");
	// print_vector(vector);
	// puts("original vect\n\n\n");
	// print_vector(x_values);
	// puts("original x_vals\n\n\n");
	x_values->values = execute_gaussian_elimination(&matrix, &vector);
		// print_matrix(matrix);
	// puts("after execute_gaussian_elimination matrix\n\n\n");
	// print_vector(vector);
	puts("after execute_gaussian_elimination vector\n\n\n");
	print_vector(x_values);
	puts("after execute_gaussian_elimination x_values\n\n\n");
	Vector * resulting_vector = multiply_matrix_by_x_vector(matrix_original, x_values, n);
	resulting_vector = subtract_vectors(resulting_vector, vector_original);
	double end_time = get_current_time();


	printf("%.7le\n", l2_norm(resulting_vector));
	printf("Completed in %.10lfsec\n", end_time - start_time);
	return 0;
}