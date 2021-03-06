/*
Yaseen Aniss
This is matrix.h file. It is used to define what a Matrix object is 
and what functions are associated with matricies
*/
#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef struct Matrix {
    int size;
    double ** values;
} Matrix;

Matrix * create_matrix(int size, int initialize);
void destroy_matrix(Matrix * matrix);
void print_matrix(Matrix * matrix);
void swap_matrix_row(Matrix **matrix, int initial_row, int swap_row);
Matrix * copy_matrix(Matrix * m);

#endif
