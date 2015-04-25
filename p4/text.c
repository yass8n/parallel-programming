#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
int main(){
  int k, i;
  double t_start, t_end;

#pragma omp parallel num_threads(num_threads) private(k, i)
  {
    /* k loops over the diagonal */
    for (k = 0; k < 10; k++)
    {
      t_start = get_time_in_sec() * 1000; /* milliseconds */       

#pragma omp single
      printf("%d K :)", k);
#pragma omp barrier
      /* perform gaussian elimination on all of the rows of this col */
#pragma omp for schedule(dynamic, 1)
      for (i = k + 1; i < 10; i++) {
        int my_rank = omp_get_thread_num();
        int thread_count = omp_get_num_threads();

        printf("Hello from thread %d of %d\n", my_rank, thread_count);
      }

      //t_end = get_time_in_sec() * 1000;
      //printf("k = %d - %.8lfms\n", k, t_end - t_start);
    }
  }
}
