#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "data.h"

#define ONLY 0


int main(int argc, char **argv) {
    MPI_Init(&argc,&argv) ;
    int width, height, myid ;
    double p ;
    Process process ;
    MPI_Comm_rank(MPI_COMM_WORLD,&myid) ;
    if(myid==ONLY) {
        scanf("%d",&width);
        scanf("%d",&height);
        scanf("%lf",&p);
    }
    MPI_Bcast(&width, 1, MPI_INT, ONLY, MPI_COMM_WORLD) ;
    MPI_Bcast(&height, 1, MPI_INT, ONLY, MPI_COMM_WORLD) ;
    MPI_Bcast(&p, 1, MPI_DOUBLE, ONLY, MPI_COMM_WORLD) ;
    /* MPI programs start with MPI_Init; all 'N' processes exist thereafter */
    process = initProcess(myid,width,height,p) ;

    printf("%d %d\n",process->gridWidth,process->gridHeight) ;

    free(process);
    MPI_Finalize() ;
    return 0 ;
}
