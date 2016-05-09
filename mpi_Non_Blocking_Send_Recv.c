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
MPI_Status status[2];
MPI_Request reqs[2];

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
} else {
	partner = rank - size/2 ;
}

MPI_Irecv(&message,1,MPI_INT,partner,1,MPI_COMM_WORLD,&reqs[0]);	 
printf("%s : Receiving message from %d\n", hostname, partner);
MPI_Isend(&rank,1,MPI_INT,partner,1,MPI_COMM_WORLD,&reqs[1]);
printf("%s : Sending message to %d\n", hostname, partner);

MPI_Waitall(2,reqs,status);
MPI_Finalize();
return 1;
}
