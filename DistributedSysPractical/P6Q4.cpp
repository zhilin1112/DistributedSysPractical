// P6Q4.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "mpi.h"
#include "iostream"

int main(int argc, char** argv)
{
    int rank = 0, value = 0, size = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    //Get process ID
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Get total processes Number
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    

    do {
        if (rank == 0) {
            printf("Input value (-1 to exit): ");
            scanf_s("%d", &value);
            

        }
        else {
            MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
            if (rank < size - 1) {
                MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            }
            

        }
        printf("Process %d got %d\n", rank, value);
        fflush(stdout);


    } while (value >= 0);

    MPI_Finalize();
    return 0;
}