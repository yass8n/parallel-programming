/*
Yaseen Aniss
This is vector.c file. It is used to define the functions associated with vectors
*/
#include "vector.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int thread_count;
int chunk_size;
/*
  Purpose:      Allocate memory for the Vector 

  Input args:   Size: the number of values to allocate
  				initialize: can be 0 or 1.
  					        if 0, dont initialzie the values
  					        if 1, initialize the values to random numbers

  Output args:  Vector *: pointer to a new Vector object
 */
Vector * create_vector(int size, int initialize){
	Vector * vector = malloc(sizeof(vector));
	vector->values = calloc(sizeof(double), size);
	vector->size = size;
	if (initialize == 1){
    int i = 0;
		for (i; i < vector->size; i++){
		     vector->values[i] = drand48() * 2e6 - 1e6;
		}
	}
	return vector;
}
/*
  Purpose:      Print a Vector's values

  Input args:   vector: pointer to the vector we will be operating on
 */
void print_vector(Vector * vector){
	int i = 0;
	for (i; i < vector->size; i++){
	     printf("%f\n ", vector->values[i]);
	}
}
/*
  Purpose:      Swap the rows of a vector

  Input args:   vector: pointer to the vector we will be operating on
                initial_row: the index of one of the rows we will be swapping
                swap_row: the index of the other row we will be swapping

  Output args:  Vector *: the new vector with the updated swaps         
 */
void swap_vector_row(Vector **vector, int initial_row, int swap_row){
    double initial_value = (*vector)->values[initial_row];
	(*vector)->values[initial_row] = (*vector)->values[swap_row];
	(*vector)->values[swap_row] = initial_value;
}
/*
  Purpose:      Create an exact copy of a vector
  Input args:   vect: pointer to the vector we will be operating on

  Output args:  Vector *: the new vector that is a copy of the "vect" vector        
 */
Vector * copy_vector(Vector * vect){
    Vector * vector  = malloc(sizeof(Vector));
    
    vector->size = vect->size;
    vector->values = malloc(sizeof(double) * vector->size);
    
    memcpy(vector->values, vect->values, sizeof(double) * vector->size);
    
    return vector;
}
/*
  Purpose:      Calculate the l2-norm
  Input args:   vect: pointer to the vector we will be operating on
  Output args:  double: the value of the l2-norm      
 */
double l2_norm(Vector * vect)
{
    double result = 0;
    int i = 0;
    #pragma omp parallel for num_threads(thread_count)\
    private(i) schedule(static,chunk_size) reduction(+:result) 
    for(i=0; i < vect->size; i++){
        result += pow(vect->values[i], 2);
    }
    return cabs(sqrt(result));
}
/*
  Purpose:      Subtract the values of two vectors to create a new one 
  				with the new resulting values

  Input args:   original_vect: pointer to one of the vector we will be operating on
                resulting_vect: pointer to one of the vector we will be operating on

  Output args:  resulting_vect: Vector that contains the 
  				result of "resulting_vect - original_vect"        
 */
Vector * subtract_vectors(Vector * resulting_vect, Vector * original_vect){
	int i = 0;
  #pragma omp parallel for num_threads(thread_count) shared(original_vect, resulting_vect)\
  private(i) schedule(static,chunk_size)
	for (i = 0; i < resulting_vect->size; i++){
		resulting_vect->values[i] = (resulting_vect->values[i] - original_vect->values[i]);
	}
	return resulting_vect;
}