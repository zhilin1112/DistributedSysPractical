// mpiexec -np 4 P6Q3

#include "stdio.h"
#include "mpi.h"

int main(int argc, char** argv)
{
    int rank = 0, value = 0;

    MPI_Init(&argc, &argv);

    //Get process ID
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    do {
        if (rank == 0){
            printf("Input value (-1 to exit): ");
            scanf_s("%d", &value);
        }
        //Broadcast the input value to all the processes
        MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        printf("Process %d got %d\n", rank, value);
        fflush(stdout);
        

    } while (value >= 0);

    MPI_Finalize();

    return 0;
}