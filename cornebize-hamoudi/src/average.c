#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define _ONLY if(myid==0)

int numprocs, lengthgrid;
int myid, myrow, mycol;
int left, right, up, down ;

int main(int argc, char **argv) {
    /* MPI programs start with MPI_Init; all 'N' processes exist thereafter */
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    lengthgrid = (int)sqrt((double)numprocs) ;

    /* Handling of non-perfect-square numbers of processors. */
    if(lengthgrid*lengthgrid != numprocs)
        _ONLY fprintf(stderr,"Warning: number of processes is not a perfect square.\n") ;
    numprocs = lengthgrid*lengthgrid ;
    if(myid >= numprocs) {
        printf("Process %d is killed.\n",myid);
        MPI_Finalize() ;
        return 0 ;
    }

    printf("Process %d terminated normally.\n",myid);
    MPI_Finalize();
    return 0 ;
}
