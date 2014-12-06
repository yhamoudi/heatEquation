#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "data.h"

#define ONLY 0
#define TAG 0

void compute(Process process) {
    fprintf(stderr,"Process %d is computing...\n",process->myid) ;
    process->nbIter = process->currentIter ;
}

void io(Process process) {
    int i, buffio[3] = {0,0,0} ;
    double content ;
    MPI_Status stat;
    while(1) {
        if(process->myid == ONLY) {
            for(i = 0 ; i < 3 ; i++) {
                if(scanf("%d",&buffio[i])==EOF) {
                    buffio[0] = -1 ;
                    break ;
                }
            }
            if(scanf("%lf",&content)==EOF)
                buffio[0] = -1 ;
        }
        MPI_Bcast(buffio, 3, MPI_INT, ONLY, MPI_COMM_WORLD) ;
        MPI_Bcast(&content, 1, MPI_DOUBLE, ONLY, MPI_COMM_WORLD) ;
        if(buffio[0] == -1)
            break ;
        if(buffio[0] == 0) {
            if(setCellValue(process->automata,buffio[1],buffio[2],content))
                fprintf(stderr,"Process %d: set cell (%d,%d) to value %lf\n",process->myid,buffio[1],buffio[2],content) ;
        }
        if(buffio[0] == 2) {
            while(process->nbIter > process->currentIter) {
                compute(process) ;
            }
            content = getCell(process->automata,buffio[1],buffio[2]) ;
            if(!isnan(content) && !process->myid == ONLY) {
                MPI_Send(&content, 1, MPI_DOUBLE, ONLY, TAG, MPI_COMM_WORLD);
            }
            if(isnan(content) && process->myid == ONLY) {
                MPI_Recv(&content, 1, MPI_DOUBLE, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD,&stat);
            }
            if(process->myid == ONLY)
                printf("%lf\n",content) ;
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc,&argv) ;
    int width, height, myid, nbproc, nbiter ;
    double p ;
    Process process ;
    MPI_Comm_rank(MPI_COMM_WORLD,&myid) ;
    MPI_Comm_size(MPI_COMM_WORLD, &nbproc);
    if(myid==ONLY) {
        scanf("%d",&width);
        scanf("%d",&height);
        scanf("%lf",&p);
        scanf("%d",&nbiter);
    }
    MPI_Bcast(&width, 1, MPI_INT, ONLY, MPI_COMM_WORLD) ;
    MPI_Bcast(&height, 1, MPI_INT, ONLY, MPI_COMM_WORLD) ;
    MPI_Bcast(&p, 1, MPI_DOUBLE, ONLY, MPI_COMM_WORLD) ;
    MPI_Bcast(&nbiter, 1, MPI_INT, ONLY, MPI_COMM_WORLD) ;
    /* MPI programs start with MPI_Init; all 'N' processes exist thereafter */
    process = initProcess(myid,nbproc,width,height,p,nbiter) ;
    io(process);
    delProcess(process);
    MPI_Finalize() ;
    return 0 ;
}
