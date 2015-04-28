#include "vector.h"
#include "g_elimination.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int thread_count;
int chunk_size;
Vector * create_vector(int size, int initialize){
	Vector * vector = malloc(sizeof(vector));
	vector->values = calloc(sizeof(double), size);
	vector->size = size;
	int i = 0;
	if (initialize == 1){
		for (i; i < vector->size; i++){
		     vector->values[i] = drand48() * 2e6 - 1e6;
		}
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
Vector * copy_vector(Vector * vect){
    Vector * vector  = malloc(sizeof(Vector));
    
    vector->size = vect->size;
    vector->values = malloc(sizeof(double) * vector->size);
    
    memcpy(vector->values, vect->values, sizeof(double) * vector->size);
    
    return vector;
}
double l2_norm(Vector * vect)
{
    double result = 0;
    int i = 0;
    for(i; i < vect->size; i++){
        result += pow(vect->values[i], 2);
    }
    
    return cabs(sqrt(result));
}
Vector * subtract_vectors(Vector * resulting_vect, Vector * original_vect){
	int i;
	#pragma omp parallel for num_threads(thread_count) shared(original_vect, resulting_vect)\
	private(i) schedule(static,chunk_size)
	for (i=0; i < resulting_vect->size; i++){
		resulting_vect->values[i] -= original_vect->values[i];
	}
	return resulting_vect;
}
