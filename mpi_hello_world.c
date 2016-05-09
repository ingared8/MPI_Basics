/*
 *  Hello world Program 
 */
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {

int rank;
int size;
char hostname[255];

MPI_Init(NULL,NULL);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
gethostname(hostname,255);

printf("Hello world ! I am process number: %d on host %s\n", rank, hostname);
MPI_Finalize();
return 1;
}
