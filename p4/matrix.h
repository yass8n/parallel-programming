#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef struct Matrix {
    int size;
    double ** values;
} Matrix;

Matrix * create_matrix();
void destroy_matrix(Matrix * matrix);

#endif
