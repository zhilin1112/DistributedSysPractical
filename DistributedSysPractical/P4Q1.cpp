// P4Q1.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "omp.h"
#define NUM_THREADS 4
static long num_steps = 100000;
double step;

int main()
{
	int i, nthreads;
	double pi = 0;
	double partial_sums, sum = 0.0;
	step = 1.0 / (double)num_steps;
	omp_set_num_threads(NUM_THREADS);
	double start_time = omp_get_wtime();
#pragma omp parallel
	{
		int i, id, nthrds;
		double x;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) {
			nthreads = nthrds;
			printf("Total OMP threads: %d\n", nthreads);
		}

		//It will have 4 loops in the 4 threads 
		// i = 0 ; then 0 + 4 (nthrds) i = 1; 1 + 4 --> Skipped 4 
		//To collect the partial sums with multiple threads
		for (i = id, partial_sums = 0.0; i < num_steps; i = i + nthrds)
		{
			x = (i + 0.5) * step;
			partial_sums = 4.0 / (1.0 + x * x);

			#pragma omp critical 
			{
				sum += partial_sums;
			}
		}
	}
	////Outside parallel not run parallely 
	//for (i = 0, pi = 0.0; i < nthreads; i++) {
	//    sum += partial_sums[i];
	//}
	pi = sum * step;
	double end_time = omp_get_wtime();

	printf("%f\n", pi);

	printf("Work took %f seconds\n", end_time - start_time);
	return 0;
}