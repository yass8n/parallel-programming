#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "matrix.h"
/*
 * Function:  get_input_from_user
 * Purpose:   Reads in the users input and summarizes how to run program
              if the user fails to pass the correct number of arguments
 * In args:   argc, argv
 * Out args:  n: number of rows and columns
              thread_count: number of threads tp use
 */
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
	// drand48_r();
	Vector * vector = create_vector(n);
	Matrix * matrix = create_matrix(n);
	print_matrix(matrix);
	print_vector(vector);
	return 0;
}