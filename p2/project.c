//Yaseen Aniss

#include "project.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <complex.h>

/* returns the value of the function with x as the input */
static double func_to_integrate(double x){
  return cos(x/3) - 2 * cos(x/5) + 5 * sin(x / 4) + 8;
}
/*initializes the project object pointer by setting its function, true value, and minimum acceptable error */
void initialize_project(project * proj){
    proj->f_of_x   = func_to_integrate;
    proj->true_val = 4003.7209001513268;
    proj->min_error_val = .000000000000005; // equal to 5.0 * 10^(-15)
}

/* calculates the value of the width
a = beginning of range, b = end of range, n = number of trapezoids to use for the range */
double calc_h(double a, double b, int n) {
  return ((b - a) / (double) n);
}

/* calculates the absolute true error by taking the true value of the approximated
value of the integral and subtracting the true value, then dividing by the approx value */
double calc_abs_true_error(double true_val, double approx_val) {
    return fabs((approx_val- true_val) / approx_val);
}

/* calculates the value of the function for the given range (a-b /n) */
double trapezoidal_estimate(project * proj, double a, double b, double n){
    double h         = calc_h(a, b, n);
    double inner_sum = (proj->f_of_x(a) + proj->f_of_x(b))/2;
    /* calculate the inner sum */
    for (int i = 1; i <= n - 1; i++) {
        double val = a + i * h;
        inner_sum += proj->f_of_x(val);
    }
    return inner_sum * h;
}

/*  This function returns true if we reached the criteria for the true error value and 
our approximated value is precisie to 14 significant digits */
int is_goal_reached(double abs_true_error, double min_error, double true_val, double approx){
  if (abs_true_error <= min_error && fourteen_digit_comparison(true_val, approx))
    return 1;
  else
    return 0;
}

/*  This function returns true when
    we have reached a precision of 14 significant figures. It makes this decision by 
    checking that all the first 14 digits are 0 when we do a subtraction of the true_val - approx_val
    */
int fourteen_digit_comparison(double true_val, double approx){
  double difference     = cabs(true_val - approx);
  double truncated_diff =  trunc(difference * 100000000000);
  if (truncated_diff == 0.0)
    return 1;
  else
    return 0;
}