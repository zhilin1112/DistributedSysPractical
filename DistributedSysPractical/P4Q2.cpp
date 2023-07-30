// P4Q2.cpp : Defines the entry point for the console application.
//test

#include <stdio.h>
#include <cstdlib>
#include "omp.h"
#define NUM_THREADS 4

const long MAX = 100000;

int oriMethod() {
	double ave = 0.0, * A;
	int i;

	A = (double*)malloc(MAX * sizeof(double));
	if (A == NULL) {
		printf("Insufficient memory! Can't continue. Terminating the program abruptly.\n");
		return -1;
	}

	for (i = 0; i < MAX; i++)
	{
		A[i] = (double)i;
	}
	double start_time = omp_get_wtime();

	for (i = 0; i < MAX; i++) {
		ave += A[i];
	}
	double end_time = omp_get_wtime();
	ave = ave / MAX;
	printf("%f\n", ave);
	printf("Original work took %f seconds\n", end_time - start_time);
	free(A);
	return 0;
}

int criticalMethod() {
	double ave = 0.0;

	omp_set_num_threads(NUM_THREADS);
	double start_time = omp_get_wtime();
#pragma omp parallel
	{
		double sum = 0.0;
		int num = omp_get_num_threads();
		int thread = omp_get_thread_num();
		for (int i = thread; i < MAX; i += num)
		{
			sum += i;
		}
#pragma omp critical
		{
			ave += sum / MAX;
		}
	}
	double end_time = omp_get_wtime();
	printf("%f\n", ave);
	printf("Original work took %f seconds\n", end_time - start_time);
	return 0;
}

int reductionMethod() {
	double ave = 0.0;

	omp_set_num_threads(NUM_THREADS);
	double start_time = omp_get_wtime();
	int num = omp_get_num_threads();
	int thread = omp_get_thread_num();
#pragma omp parallel for reduction(+ : ave)
	for (int i = thread; i < MAX; i += num)
	{
		ave += i;
	}
	double end_time = omp_get_wtime();
	ave = ave / MAX;
	printf("%f\n", ave);
	printf("Original work took %f seconds\n", end_time - start_time);
	return 0;
}

int main()
{
	oriMethod();
	criticalMethod();
	reductionMethod();
	return 0;
}