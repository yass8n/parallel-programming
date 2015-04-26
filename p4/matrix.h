#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef struct Matrix {
    int size;
    double ** values;
} Matrix;

Matrix * create_matrix();
void destroy_matrix(Matrix * matrix);
void print_matrix(Matrix * matrix);
void swap_matrix_row(Matrix **matrix, int initial_row, int swap_row);

#endif
