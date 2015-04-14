#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <mpi.h>

//http://stackoverflow.com/questions/2128728/allocate-matrix-in-c
//using the above reference for a simulation of a matrix...example below
/*
arr[offset] where offset is = i * num_cols + j
instead of arr[row][cols] //row major

AND

arr[offset] where offset is = i + num_rows * j
instead of arr[cols][rows] //column major
*/
/*
 * Purpose: Build a derived datatype so that the three
 *          values can be sent in a single MPI message.
 * Input args:   g_values: pointer to the trapezoidal_values input data
 * Output args:  input_mpi_t_p:  the new MPI datatype that is created
 */
 typedef struct Details {
    char matrix_form[4];//buffer for the ijk, ikj, or kij
    char flag[1];//buffer for the flag (R or I)
    int n; //matrix is size n X n
} Details;

static void build_mpi_type(Details *details, MPI_Datatype * input_details){
    int array_of_blocklengths[3] = {4, 1, 1};
    MPI_Datatype array_of_types[3] = {MPI_CHAR, MPI_CHAR, MPI_INT};
    MPI_Aint m_addr, f_addr, n_addr;
    MPI_Aint array_of_displacements[3] = {0};

    MPI_Get_address(&details->matrix_form, &m_addr);
    MPI_Get_address(&details->flag, &f_addr);
    MPI_Get_address(&details->n, &n_addr);
    array_of_displacements[1] = f_addr-m_addr;
    array_of_displacements[2] = n_addr-m_addr;
    MPI_Type_create_struct(3,array_of_blocklengths,array_of_displacements, 
                           array_of_types,input_details);
    MPI_Type_commit(input_details);
}

int main(){
	// int *global_A, *local_A, *global_B, *final_matrix;

	/* Initialize MPI */
    MPI_Init(NULL, NULL);

    // declaring the rank and comm size variables for all processors to have
	int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int comm_sz;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    MPI_Datatype input_details;
	// Details *details = malloc(sizeof(Details));
	Details details;
    build_mpi_type(&details, &input_details);

    if (my_rank == 0){
    	details.n = 5;
    	fscanf(stdin, "%s", details.matrix_form);
    	fscanf(stdin, "%s", details.flag);
    	details.n = 5;
    	// fscanf(stdin, "%d", &details->n );

    	// fscanf(stdin, "%s", details->matrix_form);
    	// fscanf(stdin, "%s", details->flag);
    }
    /* Send the data from rank 0 to other processors/threads  */
    MPI_Bcast(&details, 1, input_details, 0, MPI_COMM_WORLD);

    if (my_rank == 1){
    	printf("%d\n", details.n);
    	printf("%s\n", details.matrix_form);
    	printf("%s\n", details.flag);
    }

    MPI_Finalize();

}