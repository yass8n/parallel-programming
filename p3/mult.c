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
void get_user_input(int argc, char *argv[], matrix *A, matrix *B, form_and_flag *form_flag) {
	fscanf(stdin, "%s", form_flag->matrix_form); //get form
	fscanf(stdin, "%s", form_flag->flag);
	fscanf(stdin, "%d", &(form_flag->n));
	while (!feof(stdin)){
		char form_buf[10] = "";
		fscanf(stdin, "%3s", form_buf);
	}

}

int main(int argc, char * argv[]){
	matrix A, B, C;
	form_and_flag form_flag;
	get_user_input(argc, argv, &A, &B, &form_flag);
}
