/*
Yaseen Aniss
This is g_elimination.c file. It is used to execute the gaussian elimination algorithm
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "g_elimination.h"
#include <omp.h>

int thread_count = 1;
int chunk_size = 1;
/*
  Purpose:      Set the global thread count variable
  
  Input args:   num_threads: the number of threads to use
  				n: the number of values in a row
 */
void set_thread_count_and_chunk_size(int num_threads, int n){
	thread_count = num_threads;
	if (n % (thread_count*2) != 0){
		chunk_size = calculate_chunk_size(thread_count, n);
	} else {
		chunk_size = ((n / (thread_count*2)));
	}
}
/*
  Purpose:      Calculate the next highest chunk size that is evenly divisible by n
  
  Input args:   num_threads: the number of threads to use
  				n: the number of values in a row

  returns: next highest chunk size that is evenly divisible by n 
		  ...this number will be slightly less than (n / (thread_count*2)) at best case
		  OR equal to 1 at worst case 
 */
int calculate_chunk_size(int num_threads, int n){ 
  int i;
  for (i = num_threads; i < n; i++){
    if (n % (i*2) == 0){ 
      return n/(i*2);
    }   
  }
  //worst case scenaio: we return 1 as the chunk size if all else fails
  return 1;
}
/*
  Purpose:      For value in the current column that has an index greater than "row",
                use forward elimination to set that value to 0
  
  Input args:   matrix: the matrix we will be operating on
                vector: the vector we will be operating on
  				row: the row to start at

  Output args:  matrix *: the new matrix with the updated column
                vector *: the new vector with the updated column
 */
void forward_elimination(Matrix **matrix, Vector **vector, int row){
	int static_column = row;
	#pragma omp parallel for num_threads(thread_count) shared(matrix, vector, static_column)\
	 private(row) schedule(static,chunk_size)
	for (row = static_column + 1; row < (*matrix)->size; row++){
		double multiplier = (*matrix)->values[row][static_column] / (*matrix)->values[static_column][static_column];
	    int new_column = static_column;
		for (new_column = new_column; new_column < (*matrix)->size; new_column++){
				(*matrix)->values[row][new_column] = (*matrix)->values[row][new_column] - ((*matrix)->values[static_column][new_column] * multiplier);
		}
	    (*vector)->values[row] = (*vector)->values[row] - (multiplier * (*vector)->values[static_column]);
	}
}
/*
  Purpose:      Calculate the X values for the linear system 
  
  Input args:   matrix: the matrix we will be operating on
                vector: the vector we will be operating on

  returns:  x_values: the new vector with the X values that
                          solve the linear system
 */
double * back_substitution(Matrix **matrix, Vector **vector){
	int i;
	double * x_values = calloc(sizeof(double), (*matrix)->size);
	for (i = (*matrix)->size-1; i >= 0; i--){
		x_values[i] = (*vector)->values[i]/(*matrix)->values[i][i];
		double x_value = x_values[i]; 
		//saving the value in a variable (x_value) improves 
		//timing because in the for loop ahead because we no longer have to index the x_values array n times
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
/*
  Purpose:      Calculate the cross product of "matrix_row_values" and "x_values"
  
  Input args:   matrix_row_values: the matrix row we will be operating on
                x_values: array of X values that
                          solve the linear system
                size: length of both "matrix_row_values" and "x_values"

  returns:  result: the result of the cross product between "matrix_row_values" and "x_values"
 */
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
/*
  Purpose:      Calculate the multiplication of a matrix and a vector
  
  Input args:   matrix: the matrix we will be operating on
                x_values: vector of X values that
                          solve the linear system
                n: length of the x_values and matrix rows

  returns:  result: the result of the multiplication between the matrix and the vector
 */
Vector * multiply_matrix_by_x_vector(Matrix * matrix, Vector * x_values, int n){
	Vector * result = create_vector(x_values->size, 0);
	int i;
	#pragma omp parallel for num_threads(thread_count) shared(matrix, x_values, result)\
	 private(i) schedule(static,chunk_size)
	for (i = 0; i < x_values->size; i++){
		result->values[i] = cross_product(matrix->values[i], x_values->values, n);
	}
	return result;
}
/*
  Purpose:      Run the gaussian elimination algorithm
  
  Input args:   matrix: the matrix we will be operating on
                vector: the vector we will be operating on

  returns:  x_values: the new vector with the X values that
                          solve the linear system
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
