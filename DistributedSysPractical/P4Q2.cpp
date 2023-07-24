// P4Q2.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <cstdlib>
#include "omp.h"
#define NUM_THREADS 4
static long num_steps = 100000;
double step;

const long MAX = 100000;

int serial()
{
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

int reduction()
{
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

#pragma omp parallel for reduction(+:ave)
    for (i = 0; i < MAX; i++) {
        ave += A[i];
    }
    double end_time = omp_get_wtime();
    ave = ave / MAX;
    printf("%f\n", ave);
    printf("Reduction work took %f seconds\n", end_time - start_time);
    free(A);
    return 0;
}

int criticalRegion()
{
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

#pragma omp parallel for shared(ave)
    for (i = 0; i < MAX; i++) {
#pragma omp critical
        ave += A[i];
    }

    double end_time = omp_get_wtime();
    ave = ave / MAX;
    printf("%f\n", ave);
    printf("Critical work with omp critical took %f seconds\n", end_time - start_time);
    free(A);
    return 0;
}

int main() {
    serial();
	reduction();
	criticalRegion();
    return 0;
}