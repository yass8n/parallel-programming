#include "matrix.h"
#include <stdlib.h>
#include "vector.h"
#include <stdio.h>
#include <string.h>
Matrix * create_matrix(int size){
	Matrix * matrix = malloc(sizeof(Matrix));
	matrix->size = size;
	int i = 0, j = 0;
	matrix->values = malloc(sizeof(double *) * matrix->size);
	for (i; i < matrix->size; i++){
		matrix->values[i] = calloc(sizeof(double), matrix->size);
		for (j = 0; j < matrix->size; j++){
			matrix->values[i][j] = drand48() * 2e6 - 1e6;
		}
	}
	return matrix;
}
void print_matrix(Matrix * matrix){
	int i = 0, j = 0;
	for (i; i < matrix->size; i++){
		for (j = 0; j < matrix->size; j++){
			printf("%f ", matrix->values[i][j]);
		}
		puts("\n");
	}
}