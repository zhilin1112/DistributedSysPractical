// P6Q4.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    int rank, value = 0, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    do {
        if (rank == 0) {
            scanf_s("%d", &value);
            MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
        else {
            MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
            if (rank < size - 1)
                MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
        printf("Process %d got %d\n", rank, value);
        fflush(stdout);
    } while (value >= 0);

    MPI_Finalize();
    return 0;
}

//MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
//MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);