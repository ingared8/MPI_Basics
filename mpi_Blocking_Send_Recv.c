//Program Name
// Other Comments
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#define MASTER 0

int main(int argc, char **argv) {

int rank;
int size;
int partner; 
int message;
int name_size=MPI_MAX_PROCESSOR_NAME;
char hostname[name_size];
MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
gethostname(hostname,name_size);

// Check for master
if (rank == MASTER){
printf("Master Node: %s: Total  No of processors is: %d\n", hostname,size);
} 
printf("Generic Node: %s : Rank is %d \n",  hostname, rank);


// split the nodes in to 2 groups of equal sizes and send messages between them.
// i th in group1 node will send message to (n/2+i) 
if ( rank < size/2){
	partner = rank + size/2 ;
	MPI_Send(&rank,1,MPI_INT,partner,1,MPI_COMM_WORLD);
	printf("%s : Sent message to %d\n", hostname, partner);
	MPI_Recv(&message,1,MPI_INT,partner,1,MPI_COMM_WORLD,&status);
	printf("%s : Received  message from %d\n", hostname, partner);
} else {
	partner = rank - size/2 ;
	MPI_Recv(&message,1,MPI_INT,partner,1,MPI_COMM_WORLD,&status);	 
	printf("%s : Received message from %d\n", hostname, partner);
	MPI_Send(&rank,1,MPI_INT,partner,1,MPI_COMM_WORLD);
	printf("%s : Sent message to %d\n", hostname, partner);
}

MPI_Finalize();
return 1;
}
