#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct Vector {
    double * values;
    int size;
} Vector;

Vector * create_vector(int size);
void destroy_vector(Vector * vector);
void print_vector(Vector * vector);
void swap_vector_row(Vector **vector, int initial_row, int swap_row);

#endif
