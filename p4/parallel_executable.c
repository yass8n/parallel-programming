#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "matrix.h"
#include "g_elimination.h"
#include <assert.h>
#include <omp.h>
/*
 * Function:  get_input_from_user
 * Purpose:   Reads in the users input and summarizes how to run program
              if the user fails to pass the correct number of arguments
 * In args:   argc, argv
 * Out args:  n: number of rows and columns
              thread_count: number of threads to use
 */

void get_input_from_user(int argc, char * argv[], int * n){
	if (argc < 3){
		fprintf(stderr, "Must run '%s' with following arguments: <thread count> <n> <g|i>\n", argv[0]);
		exit(0);
	}
	(*n) = atoi(argv[1]);
    thread_count = atoi(argv[2]);
}
double get_current_time()
{
    struct timeval t; 
    gettimeofday(&t, NULL);
    return t.tv_sec + (t.tv_usec / (double) 1000000.0);
}
int main(int argc, char * argv[]){
	int n;
	get_input_from_user(argc, argv, &n);
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
	// puts("after execute_gaussian_elimination vector\n\n\n");
	print_vector(x_values);
	puts("after execute_gaussian_elimination x_values\n\n\n");
	Vector * resulting_vector = multiply_matrix_by_x_vector(matrix_original, x_values, n);
	resulting_vector = subtract_vectors(resulting_vector, vector_original);
	// print_vector(resulting_vector);
	// puts("after execute_gaussian_elimination resulting_vector\n\n\n");
	double end_time = get_current_time();


	printf("%.7le\n", l2_norm(resulting_vector));
	printf("Completed in %.10lfsec\n", end_time - start_time);
	return 0;
}