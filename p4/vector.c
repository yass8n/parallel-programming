#include "vector.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Vector * create_vector(int size){
	Vector * vector = malloc(sizeof(vector));
	vector->values = calloc(sizeof(double), size);
	vector->size = size;
	int i = 0;
	for (i; i < vector->size; i++){
	     // vector->values[i] = drand48() * 2e6 - 1e6;
	     vector->values[i] = rand() % 20;
	}
	return vector;
}
void print_vector(Vector * vector){
	int i = 0;
	for (i; i < vector->size; i++){
	     printf("%f\n ", vector->values[i]);
	}
}
void swap_vector_row(Vector **vector, int initial_row, int swap_row){
    double initial_value = (*vector)->values[initial_row];
	(*vector)->values[initial_row] = (*vector)->values[swap_row];
	(*vector)->values[swap_row] = initial_value;
}