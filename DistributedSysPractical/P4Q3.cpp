// P4Q3.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <cstdlib>
#include "omp.h"

const int N = 100000;

//Do this first --> Producer 
void fill_rand(int nval, double* A) {
	for (int i = 0; i < nval; i++) A[i] = (double)rand();
}

//Consumer 
double Sum_array(int nval, double* A) {
	double sum = 0.0;
	for (int i = 0; i < nval; i++)
		sum = sum + A[i];
	return sum;
}

int oriMethod() {
	double* A, sum, runtime;
	int flag = 0;

	A = (double*)malloc(N * sizeof(double));
	srand(100);

	runtime = omp_get_wtime(); //Starting time 
	fill_rand(N, A);	// Producer: fill an array of data 
	sum = Sum_array(N, A);	// Consumer: sum the array 
	runtime = omp_get_wtime() - runtime; // How long it takes

	printf(" In %lf seconds, The sum is %lf \n", runtime, sum);

	return 0;
}

int parallelSection() {
	double* A, sum, runtime;
	int flag = 0;

	A = (double*)malloc(N * sizeof(double));
	srand(100);

	runtime = omp_get_wtime(); //Starting time 

#pragma omp parallel sections 
	{
#pragma omp section
		{
			fill_rand(N, A); // Producer: fill an array of data 
#pragma omp flush
			flag = 1;
#pragma omp flush(flag)

		}
#pragma omp section
		{
#pragma omp flush(flag)
			while (flag != 1) {
#pragma omp flush(flag)
			}
#pragma omp flush
			sum = Sum_array(N, A);	// Consumer: sum the array
		}
	}
		 
		runtime = omp_get_wtime() - runtime; // How long it takes

		printf(" In %lf seconds, The sum is %lf \n", runtime, sum);

		return 0;
}
int main() {
	oriMethod();
	parallelSection();
	return 0;
}
