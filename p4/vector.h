/*
Yaseen Aniss
This is vector.h file. It is used to define what a Vector object is 
and what functions are associated with vectors
*/
#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <complex.h>

typedef struct Vector {
    double * values;
    int size;
} Vector;

Vector * create_vector(int size, int initialize);
void destroy_vector(Vector * vector);
void print_vector(Vector * vector);
void swap_vector_row(Vector **vector, int initial_row, int swap_row);
Vector * copy_vector(Vector * vect);
double l2_norm(Vector * vect);
Vector * subtract_vectors(Vector * resulting_vect, Vector * original_vect);

#endif
