#ifndef _G_ELIMINATION_H_
#define _G_ELIMINATION_H_
#include "vector.h"
#include "matrix.h"

extern int thread_count;
extern int chunk_size;
void forward_elimination(Matrix **matrix, Vector **vector, int row);
double calculate_right_side(double *row, double right_side_value, 
	                        double *x_values, int current_column, int size);
double * back_substitution(Matrix **matrix, Vector **vector);
double * execute_gaussian_elimination(Matrix ** matrix, Vector ** vector);
double cross_product(double * matrix_row_values, double * x_values, int size);
Vector * multiply_matrix_by_x_vector(Matrix * matrix, Vector * x_values, int n);
void set_thread_count(int num_threads, int n);
int greatest_divisible_denominator(int a,int b);

#endif
