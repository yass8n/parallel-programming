//Yaseen Aniss
/*
 * http://www.ecst.csuchico.edu/~judyc/1314S-csci551/code/Pacheco/ch3/mpi_trap4.c
 * I followed and referenced the structure of Peter Pacheco's 'mpi_trap4.c' from the above URL.
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "project.h"

/* an object to hold the values of a, b and n 
* this object will be used for the internal values and the global values in this file */
    typedef struct {
        double a;
        double b;
        int n;
    } trapezoidal_values;

/*------------------------------------------------------------------
 * Most of this function was written by Peter Pacheco!
 * Function: Build_mpi_type
 * Purpose: Build a derived datatype so that the three
 *          values can be sent in a single MPI message.
 * Input args:   g_values: pointer to the trapezoidal_values input data
 * Output args:  input_mpi_t_p:  the new MPI datatype that is created
 */
static void build_mpi_type(trapezoidal_values * g_values, MPI_Datatype * input_mpi_t_p){
    int array_of_blocklengths[3] = {1, 1, 1};
    MPI_Datatype array_of_types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
    MPI_Aint a_addr, b_addr, n_addr;
    MPI_Aint array_of_displacements[3] = {0};

    MPI_Get_address(&g_values->a, &a_addr);
    MPI_Get_address(&g_values->b, &b_addr);
    MPI_Get_address(&g_values->n, &n_addr);
    array_of_displacements[1] = b_addr-a_addr;
    array_of_displacements[2] = n_addr-a_addr;
    MPI_Type_create_struct(3,array_of_blocklengths,array_of_displacements, 
                           array_of_types,input_mpi_t_p);
    MPI_Type_commit(input_mpi_t_p);
}

int main(){   
    /* allocate memory for the trapezoidal_values objects */
    trapezoidal_values * local_input = malloc(sizeof(trapezoidal_values)); 
    trapezoidal_values * global_input = malloc(sizeof(trapezoidal_values));

    /* allocate memory for and initialize the project object */
    project * proj = malloc(sizeof(project));
    initialize_project(proj);

    /* Initialize MPI */
    MPI_Init(NULL, NULL);

    /* set the rank of the current process and save the value in 'my_rank' */
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /*set the value of how many processors are being used in 'comm_sz' */
    int comm_sz;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* create the trapezoidal_values derived data type to use in the MPI function 'MPI_Bcast' */
    MPI_Datatype input_mpi_t;
    build_mpi_type(global_input, &input_mpi_t);

    /* fill the global_input object with the user's input of a, b, and n...only if we are process 0 */
    if (my_rank == 0){
        puts("Enter int values for a, b, and n");
        int num_inputs= scanf("%lf %lf %d", &global_input->a, &global_input->b, &global_input->n);

        while (num_inputs != 3){ //sometimes the script "create_output.rb" doesn't send all the values, or sends too much...so try again
            puts("Enter int values for a, b, and n");
            num_inputs = scanf("%lf %lf %d", &global_input->a, &global_input->b, &global_input->n);
        }
    }

    /* Send the data from rank 0 to other processors/threads  */
    MPI_Bcast(global_input, 1, input_mpi_t, 0, MPI_COMM_WORLD);

    /* Free the derived data type we created from build_mpi_type */
    MPI_Type_free(&input_mpi_t);

    /* MPI_Barrier blocks until all processors have reached here */
    MPI_Barrier(MPI_COMM_WORLD);

    /* start the timer on rank 0 */
    double start_time;
    if (my_rank == 0) {
        start_time = MPI_Wtime();
    }

    /* set the local input for each processor */
    local_input->n = global_input->n / comm_sz; // dividing the problem size up by num of processes   
    double h = calc_h(global_input->a, global_input->b, global_input->n); //calculating the width
    local_input->a = global_input->a + my_rank * local_input ->n * h;  // calculating leftside local interval   
    local_input->b = local_input ->a + local_input ->n * h; // calculating rightside local interval   

    /* do the local trapezoidal calculation for each processor*/
    double local_int = trapezoidal_estimate(proj, local_input->a, local_input->b, local_input->n);

    /* Add up the integrals calculated by each process and save the final value in 'total_int' */
    double total_int;
    MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    /* stop the timer, calculate the elapsed time, and print the results ... only if we are rank 0*/
    double finish_time;
    if (my_rank == 0) {
        finish_time  = MPI_Wtime();
        double elapsed_time = finish_time - start_time;
        double abs_true_error = calc_abs_true_error(proj->true_val, total_int);
        if (abs_true_error >= proj->min_error_val){
            printf("\nRunning on %d processor(s).\n""Elapsed time = %.7e seconds\n"
            "With n = %d trapezoid(s),\n""our estimate of the integral from %lf to %lf = %.14e\n"
            "absolute relative true error = %e is NOT less than criteria = %e\n",
            comm_sz,elapsed_time,
            global_input->n,global_input->a, global_input->b, total_int,
            abs_true_error, proj->min_error_val);
        } else {
            printf("\nRunning on %d processor(s).\n""Elapsed time = %.7e seconds\n"
            "With n = %d trapezoid(s),\n""our estimate of the integral from %lf to %lf = %.14e\n"
            "absolute relative true error = %e is less than criteria = %e\n",
            comm_sz,elapsed_time,
            global_input->n,global_input->a, global_input->b, total_int,
            abs_true_error, proj->min_error_val);
        }
    }

    /* Close and clean MPI */
    MPI_Finalize();

    /* free the project object */
    free(proj);

    return 0;
} 