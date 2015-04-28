#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "g_elimination.h"
#include <assert.h>
#include <omp.h>

int thread_count = 1;
int chunk_size = 1;
void set_thread_count(int num_threads, int n){
	thread_count = num_threads;
	// int gdd = greatest_divisible_denominator(thread_count, n);
	chunk_size = ((n / (thread_count*2)));
}
int greatest_divisible_denominator(int smaller,int greater)
{
	int i;
	for (i = smaller; i > 0; i--){
		if (greater % smaller == 0){
			return smaller;
		}
	}
}
void forward_elimination(Matrix **matrix, Vector **vector, int row){
	int static_column = row;
	 //looping through each row of the matrix starting from the one below our target
	#pragma omp parallel for num_threads(thread_count) shared(matrix, vector, static_column)\
	 private(row) schedule(static,chunk_size)
	for (row = static_column + 1; row < (*matrix)->size; row++){
	    // printf("%d  num threads\n", omp_get_num_threads() );
	    // printf("%d  MACX threads\n", omp_get_max_threads() );
	    // printf("%d  num \n", omp_get_thread_num() );
		double multiplier = (*matrix)->values[row][static_column] / (*matrix)->values[static_column][static_column];
	    int new_column = static_column;
		for (new_column = new_column; new_column < (*matrix)->size; new_column++){
				(*matrix)->values[row][new_column] = (*matrix)->values[row][new_column] - ((*matrix)->values[static_column][new_column] * multiplier);
		}
	    (*vector)->values[row] = (*vector)->values[row] - (multiplier * (*vector)->values[static_column]);
	}
}
double * back_substitution(Matrix **matrix, Vector **vector){
	int i;
	double * x_values = calloc(sizeof(double), (*matrix)->size);
	for (i = (*matrix)->size-1; i >= 0; i--){
		x_values[i] = (*vector)->values[i]/(*matrix)->values[i][i];
		double x_value = x_values[i]; //this improves timing because in the for loop ahead, we no longer have to index the x_values array n times
		int j = 0;
		#pragma omp parallel for num_threads(thread_count) shared(matrix, vector, x_values, x_value, i)\
		private(j) schedule(static,chunk_size)
		for (j = i-1; j >=0; j--){
			(*matrix)->values[j][i] *= x_value;
			(*vector)->values[j] = (*vector)->values[j] - (*matrix)->values[j][i];
		} 
	}
	return x_values;
}
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
double cross_product(double * matrix_row_values, double * x_values, int size){
	double result = 0;
	int i;
	#pragma omp parallel for num_threads(thread_count) shared(matrix_row_values, x_values)\
	private(i) schedule(static,chunk_size) reduction(+:result) 
	for (i = 0; i < size; i++){
		result += (matrix_row_values[i] * x_values[i]);
	}
	return result;
}
Vector * multiply_matrix_by_x_vector(Matrix * matrix, Vector * x_values, int n){
	Vector * result = create_vector(x_values->size, 0);
	int i;
	#pragma omp parallel for num_threads(thread_count) shared(matrix, x_values, result)\
	 private(i) schedule(static,chunk_size)
	for (i = 0; i < x_values->size; i++){
		assert(matrix->size == x_values->size);
		result->values[i] = cross_product(matrix->values[i], x_values->values, n);
	}
	return result;
}