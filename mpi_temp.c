/******************************************************************************
* FILE: mpi_array.c
* DESCRIPTION: 
*   MPI Example - Array Assignment - C Version
*   This program demonstrates a simple data decomposition. The master task
*   first initializes an array and then distributes an equal portion that
*   array to the other tasks. After the other tasks receive their portion
*   of the array, they perform an addition operation to each array element.
*   They also maintain a sum for their portion of the array. The master task 
*   does likewise with its portion of the array. As each of the non-master
*   tasks finish, they send their updated portion of the array to the master.
*   An MPI collective communication call is used to collect the sums 
*   maintained by each task.  Finally, the master task displays selected 
*   parts of the final array and the global sum of all array elements. 
*   NOTE: the number of MPI tasks must be evenly divided by 4.
****************************************************************************/
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#define MASTER 0
#define ARRAY_SIZE 1600000
float data[ARRAY_SIZE];

int main(int argc, char **argv) {

int rank;
int size;
float correctSum, derivedSum;
int name_size=MPI_MAX_PROCESSOR_NAME;
char hostname[name_size];
MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
gethostname(hostname,name_size);

if (rank == MASTER) {
// Initialize the Array
for (int i=0; i< ARRAY_SIZE; i++){data[i] = 1.0*i; correctSum += data[i]; }
}

printf("Hello world ! I am process number: %d on host %s\n", rank, hostname);
MPI_Finalize();
return 1;
}
