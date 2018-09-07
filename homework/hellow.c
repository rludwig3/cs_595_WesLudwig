/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{

//Initialize variables in all processes, *ranks pointer ensures MPI_Gather doesn't throw error on non-0 processes

    int myid;
    int size;
    int *ranks = NULL;

//MPI init, and get rank and size for each process

    MPI_Init(0, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

//  printf("DEBUG0: I am process %d of %d\n", myid+1, size);

//Initialize ranks[] on process 0 

    if (myid == 0) {
//	printf("DEBUG1\n");
	int ranks[size];
//	printf("DEBUG2: size: %d, rank address: %pn\n", size, &ranks);
    }

//MPI_Gather collects myid rank from each process in rank order into ranks[], cannot be called within if(myid ==0)
//For some reason, gather multiplies the ranks by 4 no matter the number of processes.

    MPI_Gather(&myid, 1, MPI_INT, &ranks, 1, MPI_INT, 0, MPI_COMM_WORLD); 
//  printf("DEBUG3\n");

//Make process 0 print array in reverse order, top and rank compensate for multiplication of ranks by 4

    if (myid == 0) {	
//	printf("DEBUG4\n");
/*DEBUG
	for (int j = 0; j < size; j++) {
		printf("Ranks[%d]: %d\n", j, &ranks[j]);
	}
*/
	for (int i = size; i > 0; i--) {
		int top = &ranks[i];
		int rank = top/4;
		printf("Hello world from process %d of %d\n", rank, size);
    	}
    }
    MPI_Finalize();
    return 0;
}
