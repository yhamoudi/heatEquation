#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "data.h"

#define ONLY 0
#define TAG 0

void compute(Process process, double **buff) {
    int i ;
    MPI_Status stat;
    fprintf(stderr,"Process %d is computing...\n",process->myid) ;
    /* Copy of the columns 1 and n in the buffers. */
    for(i = 1 ; i <= process->automata->height ; i++) {
        buff[0][i-1] = process->automata->cells[i][1].content ;
        buff[1][i-1] = process->automata->cells[i][process->automata->width].content ;
    }
    /* Sending of the columns. */
    MPI_Send(buff[0], process->automata->height, MPI_DOUBLE, process->left, TAG, MPI_COMM_WORLD);
    MPI_Send(buff[1], process->automata->height, MPI_DOUBLE, process->right, TAG, MPI_COMM_WORLD);
    /* Receiving of the columns. */
    MPI_Recv(buff[2], process->automata->height, MPI_DOUBLE, process->left, TAG, MPI_COMM_WORLD,&stat);
    MPI_Recv(buff[3], process->automata->height, MPI_DOUBLE, process->right, TAG, MPI_COMM_WORLD,&stat);
    /* Copy of the buffers in the columns 0 and n+1. */
    for(i = 1 ; i <= process->automata->height ; i++) {
        process->automata->cells[i][0].content = buff[2][i-1] ;
        process->automata->cells[i][process->automata->width+1].content = buff[3][i-1] ;
    }
    /* Copy of the rows 1 and n in the buffers. */
    for(i = 1 ; i <= process->automata->width ; i++) {
        buff[4][i-1] = process->automata->cells[1][i].content ;
        buff[5][i-1] = process->automata->cells[process->automata->height][i].content ;
    }
    /* Sending of the columns. */
    MPI_Send(buff[4], process->automata->width, MPI_DOUBLE, process->up, TAG, MPI_COMM_WORLD);
    MPI_Send(buff[5], process->automata->width, MPI_DOUBLE, process->down, TAG, MPI_COMM_WORLD);
    /* Receiving of the columns. */
    MPI_Recv(buff[6], process->automata->width, MPI_DOUBLE, process->up, TAG, MPI_COMM_WORLD,&stat);
    MPI_Recv(buff[7], process->automata->width, MPI_DOUBLE, process->down, TAG, MPI_COMM_WORLD,&stat);
    /* Copy of the buffers in the columns 0 and n+1. */
    for(i = 1 ; i <= process->automata->height ; i++) {
        process->automata->cells[0][i].content = buff[6][i-1] ;
        process->automata->cells[process->automata->height+1][i].content = buff[7][i-1] ;
    }
    /* Computation of the transition function. */
    delta(process->automata) ;
    process->currentIter ++ ;
}

void io(Process process) {
    int i, buffio[3] = {0,0,0} ;
    double content ;
    double *buffcolumns[8] ; /* buffers for messages */
    for(i=0 ; i < 8 ; i++)
        buffcolumns[i] = (double*) malloc(process->automata->height*sizeof(double)) ;
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
            while(process->currentIter < process->nbIter) {
                compute(process, buffcolumns) ;
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
    for(i=0 ; i < 8 ; i++)
        free(buffcolumns[i]) ;
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
