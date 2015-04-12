#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>

typedef struct matrix {
    int rows;
    int cols;
    double * values;
} matrix;
typedef struct form_and_flag {
    char matrix_form[4];//buffer for the ijk, ikj, or kij
    char flag[1];//buffer for the flag (R or I)
    int n; //matrix is size n X n
} form_and_flag;
void create_matrices(matrix **A, matrix **B, int n){
    *A = malloc(sizeof(matrix));
    *B = malloc(sizeof(matrix));
    (*A)->rows = n;
    (*B)->cols = n;
    (*A)->values = calloc(sizeof(double), n*n);
    (*B)->values = calloc(sizeof(double), n*n);
	printf("%d in here\n", (*A)->rows);
    (*A)->values[0] = 1.3333;
}
void get_user_input(matrix **A, matrix **B, form_and_flag *form_flag) {
	fscanf(stdin, "%s", form_flag->matrix_form); //get form
	fscanf(stdin, "%s", form_flag->flag);
	fscanf(stdin, "%d", &(form_flag->n));
    if (form_flag->n < 2)
    {
        printf("%s\n", "Size Must be Greater than 1!");
        exit(0);
    }
	int keep_scanning = 1;
	create_matrices(A, B, form_flag->n);
	printf("%d in here\n", (*A)->rows);
	int j = form_flag->n * form_flag->n;
	int i = form_flag->n * form_flag->n;
	while (!feof(stdin) && keep_scanning == 1){
		char form_buf[10] = "";
		fscanf(stdin, "%3s", form_buf);
	}

}

int main(int argc, char * argv[]){
	matrix *A, *B, *C;
	form_and_flag form_flag;
	get_user_input(&A, &B, &form_flag);
	printf("%d down here\n", A->values[0]);
}
