//run in cmd/powershell with mpiexec -n 4 MPI_Example
#include <iostream>
#include <mpi.h>

using namespace std;

int main(int args, char** argvs)
{
    char name[MPI_MAX_PROCESSOR_NAME];
    cout << "Hello World" << endl;
    int rank = 0, numOfProcess = 0, len = 0;
    //MPI set up execution the environment 
    MPI_Init(&args, &argvs);

    //Get the processor process id --> Number of processes (rank id)
    //MPI_COMM_WORLD: Communicator --> Used to group up all the processor together and communicate each others 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //To get the processor name --> Array character type and the size 
    MPI_Get_processor_name(name, &len);

    //To get the number of processor processes in the communication
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcess);
    cout << "Hello World from process rank(number) " << rank << " from " << numOfProcess << " on " << name << endl;
 
    //MPI end the execution environment 
    MPI_Finalize();
    return 0;
}