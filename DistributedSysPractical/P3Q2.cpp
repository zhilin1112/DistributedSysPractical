#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

#define NUM_THREADS 2
#define PAD 8
static long num_steps = 100000;
double step;
double start_time = omp_get_wtime();


void serialVersion() {
	int i;
	double x;
	double pi;
	double sum = 0.0;

	//serial computing - step by step

	step = 1.0 / (double)num_steps;
	for (i = 0; i < num_steps; i++) {
		x = (1 + 0.5) + step;
		sum = sum + 4.0 / (1.0 + x * x);
	}
	pi = step * sum;
	printf("%f", pi);
}

void parallelVersion() {
	//parallel computing
	int i, nthreads;
	double pi;
	double sum[NUM_THREADS];
	step = 1.0 / (double)num_steps;
	omp_set_num_threads(NUM_THREADS);
	double start_time = omp_get_wtime();

#pragma omp parallel
	{
		int i, id, nthrds;
		double x;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0)
			nthreads = nthrds;

		//May happened false sharing (causng the Cache line is only 1 thread can access at a same time)
		//--> To solve this using padding (All the single element array into multiple cache line so can access same variable in different cache line)
		//sum[i][pad] --> pad == that particular thread
		for (i = id, sum[id] = 0.0; i < num_steps; i = i + nthrds) {
			x = (i + 0.5) * step;
			sum[id] += 4.0 / (1.0 + x * x);
			//printf("%d", id);
		}

	}

	double end_time = omp_get_wtime();
	for (i = 0, pi = 0.0; i < nthreads; i++) {
		pi += sum[i] * step;
	}
	printf("\n%f\n", pi);
	printf("Work took %f seconds\n", end_time - start_time);
}

void parallelCacheLineVersion() {
	//parallel computing
	int i, nthreads;
	double pi;
	double sum[NUM_THREADS][PAD];
	step = 1.0 / (double)num_steps;
	omp_set_num_threads(NUM_THREADS);
	double start_time = omp_get_wtime();

#pragma omp parallel
	{
		int i, id, nthrds;
		double x;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0)
			nthreads = nthrds;

		//May happened false sharing (causng the Cache line is only 1 thread can access at a same time)
		//--> To solve this using padding (All the single element array into multiple cache line so can access same variable in different cache line)
		//sum[i][pad] --> pad == that particular thread
		for (i = id, sum[id][0] = 0.0; i < num_steps; i = i + nthrds) {
			x = (i + 0.5) * step;
			sum[id][0] += 4.0 / (1.0 + x * x);
			//printf("%d", id);
		}

	}

	double end_time = omp_get_wtime();
	for (i = 0, pi = 0.0; i < nthreads; i++) {
		pi += sum[i][0] * step;
	}
	printf("\n%f\n", pi);
	printf("Work took %f seconds\n", end_time - start_time);
}

void parallelReductionVersion() {
	//parallel computing
	int i, nthreads;
	double pi = 0.0;
	double sum = 0.0;
	step = 1.0 / (double)num_steps;
	omp_set_num_threads(NUM_THREADS);
	double start_time = omp_get_wtime();

#pragma omp parallel
	{
		int i, id, nthrds;
		double x;
		sum = 0.0;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		nthreads = nthrds;
#pragma omp for reduction(+ : sum)

		for (i = 0; i < num_steps; i++) {
			x = (i + 0.5) * step;
			sum += 4.0 / (1.0 + x * x);
			//printf("%d", id);
		}
	}

	double end_time = omp_get_wtime();

	pi += sum * step;

	printf("\n%f\n", pi);
	printf("Work took %f seconds\n", end_time - start_time);
}


int main() {
	//serialVersion();
	parallelVersion();
	parallelCacheLineVersion();
	parallelReductionVersion();
	return 0;
}