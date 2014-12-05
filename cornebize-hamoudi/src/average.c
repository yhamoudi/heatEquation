#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define _ONLY if(myid==0)

int numprocs ;
int myid;

int main(int argc, char **argv) {
    int tmp ;
    /* MPI programs start with MPI_Init; all 'N' processes exist thereafter */
    MPI_Init(&argc,&argv);
    /* find out how big the SPMD world is */
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    tmp = (int)sqrt((double)numprocs) ;
    tmp *= tmp ;
    if(tmp != numprocs)
        _ONLY fprintf(stderr,"Warning: number of processes is not a perfect square.\n") ;
    numprocs = tmp ;
    /* and this processes' rank is */
    if(myid >= numprocs) {
        printf("Process %d is killed.\n",myid);
        MPI_Finalize() ;
        return 0 ;
    }

    printf("Process %d terminated normally.\n",myid);
    MPI_Finalize();
    return 0 ;
}
