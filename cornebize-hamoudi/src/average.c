#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "data.h"

#define ONLY 0

int numprocs, lengthgrid;
int myid, myrow, mycol;
int left, right, up, down ;

AverageAutomata input(void) {
    AverageAutomata automata ;
    int width, height ;
    double p ;
    if(myid==ONLY) {
        scanf("%d",&width);
        scanf("%d",&height);
        scanf("%lf",&p);
    }
    MPI_Bcast(&width, 1, MPI_INT, ONLY, MPI_COMM_WORLD) ;
    MPI_Bcast(&height, 1, MPI_INT, ONLY, MPI_COMM_WORLD) ;
    MPI_Bcast(&p, 1, MPI_DOUBLE, ONLY, MPI_COMM_WORLD) ;
    automata = initAutomata(height,width,p) ;
    return automata ;
}

void output(AverageAutomata automata) {
    if(myid==ONLY) {
        printf("%d %d %f\n",automata->height,automata->width,automata->p) ;
    }
    else {
        printf("%d %d %f #\n",automata->height,automata->width,automata->p) ;
    }
}

int main(int argc, char **argv) {
    /* MPI programs start with MPI_Init; all 'N' processes exist thereafter */
    MPI_Init(&argc,&argv) ;
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs) ;
    MPI_Comm_rank(MPI_COMM_WORLD,&myid) ;
    AverageAutomata automata ;
    lengthgrid = (int)sqrt((double)numprocs) ;

    /* Handling of non-perfect-square numbers of processors. */
    if(lengthgrid*lengthgrid != numprocs)
    if(myid == ONLY)
        fprintf(stderr,"Warning: number of processes is not a perfect square.\n") ;
    numprocs = lengthgrid*lengthgrid ;
    if(myid >= numprocs) {
        printf("Process %d is killed.\n",myid) ;
        MPI_Finalize() ;
        return 0 ;
    }

    automata = input() ;
    output(automata) ;

    delAutomata(automata) ;
    printf("Process %d terminated normally.\n",myid) ;
    MPI_Finalize() ;
    return 0 ;
}
