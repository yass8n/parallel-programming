#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "matrix.h"
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
	drand48();
	// drand48_r();
	Vector * vector;
	Matrix * matrix;
	vector = create_vector(10);

	return 0;
}