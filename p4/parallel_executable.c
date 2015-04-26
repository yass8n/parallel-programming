#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "matrix.h"
#include "g_elimination.h"
#include <assert.h>
#include <omp.h>
//CHANGE DYNAMIC 1 TO SOMTHING ELSE
/*
 * Function:  get_input_from_user
 * Purpose:   Reads in the users input and summarizes how to run program
              if the user fails to pass the correct number of arguments
 * In args:   argc, argv
 * Out args:  n: number of rows and columns
              thread_count: number of threads to use
 */
int thread_count;
static void get_input_from_user(int argc, char * argv[], int * n){
	if (argc < 3){
		fprintf(stderr, "Must run '%s' with following arguments: <thread count> <n> <g|i>\n", argv[0]);
		exit(0);
	}
	(*n) = atoi(argv[1]);
    set_thread_count(atoi(argv[2])); //thread_count this is a defined in g_elimination.h
    omp_set_num_threads(thread_count);
}
static double get_current_time()
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
	x_values->values = execute_gaussian_elimination(&matrix, &vector);
	print_vector(x_values);
	Vector * resulting_vector = multiply_matrix_by_x_vector(matrix_original, x_values, n);
	resulting_vector = subtract_vectors(resulting_vector, vector_original);
	double end_time = get_current_time();
	print_vector(resulting_vector);


	printf("%.7le\n", l2_norm(resulting_vector));
	printf("Completed in %.10lfsec\n", end_time - start_time);
	return 0;
}