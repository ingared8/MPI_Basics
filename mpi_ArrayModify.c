/******************************************************************************
* FILE: mpi_ArrayModify.c
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
****************************************************************************/
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#define MASTER 0
#define ARRAY_SIZE 1600000
float data[ARRAY_SIZE];

float getSum(int offset, int chunk, int myid){
int i;
float mysum=0;
for (i=offset; i < offset+chunk; i++)
{
data[i] = data[i] + i*1.0;
mysum += data[i];
}
printf("Rank %d : my calculated  sum = %e\n", myid, mysum);
return(mysum);
}


int main(int argc, char **argv) {

int rank;
int size;
int dest,source;
float correctSum, derivedSum,mySum;
int name_size=MPI_MAX_PROCESSOR_NAME;
char hostname[name_size];
MPI_Status status;
int tag2=1;
int tag1=2;
int offset;
int chunkSize;
int i,j;
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
gethostname(hostname,name_size);

chunkSize = (ARRAY_SIZE/size);

if (rank == MASTER) {
	correctSum  = 0;
	// Initialize the Array
	for (i=0; i< ARRAY_SIZE; i++){
		data[i] = 1.0*i; 
		correctSum += data[i]; }	
	
	printf("Rank %d: Array Initialized and sum of Array is %e\n", rank,correctSum); 
		

	// Distribute the array amongst the nodes
	offset = chunkSize;
	for ( dest=1; dest < size; dest++){
	MPI_Send(&offset,1,MPI_INT,dest,tag1,MPI_COMM_WORLD);
	MPI_Send(&data[offset],chunkSize,MPI_FLOAT,dest,tag2,MPI_COMM_WORLD);
	printf("Rank %d: Sent %d elements to task %d offset = %d\n",rank,chunkSize,dest, offset);
	offset  += chunkSize;	
	}
	
	offset = 0;
	// Print few elements of the UnModified Array
	for (i=0; i < size; i++){
                for (j=0; j < 5; j++ ){
                printf("%e ,", data[offset+j]);
                }
        printf("...\n");
        offset += chunkSize;
        }
	

	// Master calculates its sum
	offset = 0;
	mySum = getSum(offset,chunkSize,rank);

	// Wait for responses from other ranks
	for ( dest=1; dest < size; dest++){
        MPI_Recv(&offset,1,MPI_INT,dest,tag1,MPI_COMM_WORLD,&status);
        MPI_Recv(&data[offset],chunkSize,MPI_FLOAT,dest,tag2,MPI_COMM_WORLD,&status);
        printf("Rank %d: Recv %d elements to task %d offset\n",rank,chunkSize, offset);
        offset  += chunkSize;
        }
	
	// Get Final sum
	MPI_Reduce(&mySum,&derivedSum,1,MPI_FLOAT,MPI_SUM,MASTER,MPI_COMM_WORLD);
	printf("Sample Results: \n");
	offset = 0;
	for (i=0; i < size; i++){
		for (j=0; j < 5; j++ ){
		printf("%e ,", data[offset+j]);
		}
	printf("...\n");
	offset += chunkSize;
	}
	printf("Final sum = %e *** \n", derivedSum);
}

// Non_master code
if ( rank > MASTER)
{

// Receive data from master
source = MASTER;
MPI_Recv(&offset,1,MPI_INT,source,tag1,MPI_COMM_WORLD,&status);
MPI_Recv(&data[offset],chunkSize,MPI_FLOAT, source, tag2, MPI_COMM_WORLD, &status);
mySum = getSum(offset,chunkSize,rank);
MPI_Send(&offset,1,MPI_INT,MASTER,tag1,MPI_COMM_WORLD);
MPI_Send(&data[offset], chunkSize,MPI_FLOAT,MASTER,tag2, MPI_COMM_WORLD);

MPI_Reduce(&mySum,&derivedSum,1,MPI_FLOAT,MPI_SUM,MASTER,MPI_COMM_WORLD);
}
MPI_Finalize();
return 1;
}

