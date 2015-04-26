#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "matrix.h"
#include "g_elimination.h"
#include <assert.h>
#include <omp.h>

int thread_count;
void forward_elimination(Matrix **matrix, Vector **vector, int row){
	int static_column = row;
	#pragma omp parallel for num_threads(thread_count) shared(matrix, vector, static_column)\
	 private(row) schedule(dynamic,1)
	for (row = static_column + 1; row < (*matrix)->size; row++){
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
	    free(temp_matrix_row);
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
	for (i = (*matrix)->size-1; i >= 0; i--){
		x_values[i] = 1;
		double new_x = calculate_right_side((*matrix)->values[i], (*vector)->values[i], x_values, i, (*matrix)->size-1);
		x_values[i] = new_x;
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
	return result;
}