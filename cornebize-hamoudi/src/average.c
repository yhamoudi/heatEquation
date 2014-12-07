#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "automata.h"
#include "process.h"

#define ONLY 0
#define TAG 0
#define max(a,b) (a>b ? a : b)
#define min(a,b) (a>b ? b : a)

/* Compute in a distributed fashion one step of the delta function. */
void compute(Process process, double **buff) {
    int i ;
    MPI_Request send_requests[4] = {MPI_REQUEST_NULL,MPI_REQUEST_NULL,MPI_REQUEST_NULL,MPI_REQUEST_NULL} ;
    if(process->left != process->myid) { /* need to exchange messages */
        /* Copy of the columns 1 and n in the buffers. */
        for(i = 1 ; i <= process->automata->height ; i++) {
            buff[0][i-1] = process->automata->cells[i][1].content ;
            buff[1][i-1] = process->automata->cells[i][process->automata->width].content ;
        }
        /* Sending of the columns. */
        MPI_Isend(buff[0], process->automata->height, MPI_DOUBLE, process->left, TAG, MPI_COMM_WORLD,&send_requests[0]);
        MPI_Isend(buff[1], process->automata->height, MPI_DOUBLE, process->right, TAG, MPI_COMM_WORLD,&send_requests[1]);
        /* Receiving of the columns. */
        MPI_Recv(buff[2], process->automata->height, MPI_DOUBLE, process->left, TAG, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(buff[3], process->automata->height, MPI_DOUBLE, process->right, TAG, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        /* Copy of the buffers in the columns 0 and n+1. */
        for(i = 1 ; i <= process->automata->height ; i++) {
            process->automata->cells[i][0].content = buff[2][i-1] ;
            process->automata->cells[i][process->automata->width+1].content = buff[3][i-1] ;
        }
    }
    else { /* Already have the information */
        assert(process->gridWidth == 1 && process->right == process->myid) ;
        for(i = 1 ; i <= process->automata->height ; i++) {
            process->automata->cells[i][0].content = process->automata->cells[i][process->automata->width].content ;
            process->automata->cells[i][process->automata->width+1].content = process->automata->cells[i][1].content ;
        }
    }
    /* Copy of the rows 1 and n in the buffers. */
    if(process->up != process->myid) { /* need to exchange messages */
        for(i = 1 ; i <= process->automata->width ; i++) {
            buff[4][i-1] = process->automata->cells[1][i].content ;
            buff[5][i-1] = process->automata->cells[process->automata->height][i].content ;
        }
        /* Sending of the columns. */
        MPI_Isend(buff[4], process->automata->width, MPI_DOUBLE, process->up, TAG, MPI_COMM_WORLD,&send_requests[3]);
        MPI_Isend(buff[5], process->automata->width, MPI_DOUBLE, process->down, TAG, MPI_COMM_WORLD,&send_requests[4]);
        /* Receiving of the columns. */
        MPI_Recv(buff[6], process->automata->width, MPI_DOUBLE, process->up, TAG, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(buff[7], process->automata->width, MPI_DOUBLE, process->down, TAG, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        /* Copy of the buffers in the columns 0 and n+1. */
        for(i = 1 ; i <= process->automata->width ; i++) {
            process->automata->cells[0][i].content = buff[6][i-1] ;
            process->automata->cells[process->automata->height+1][i].content = buff[7][i-1] ;
        }
    }
    else { /* Already have the information */
        assert(process->gridHeight == 1 && process->down == process->myid) ;
        for(i = 1 ; i <= process->automata->width ; i++) {
            process->automata->cells[0][i].content = process->automata->cells[process->automata->height][i].content ;
            process->automata->cells[process->automata->height+1][i].content = process->automata->cells[1][i].content ;
        }
    }
    /* Computation of the transition function. */
    delta(process->automata) ;
    process->currentIter ++ ;
    for(i = 0 ; i < 4 ; i++)
        MPI_Wait(&send_requests[i], MPI_STATUS_IGNORE) ;
}

/* Main function. */
/* Read on the standard input the input data and the requests.
 * Compute the delta transition.
 * Print on the standard output to answer to the requests. */
void inputOutput(Process process) {
    int i, buffio[3] = {0,0,0} ;
    double content ;
    double *buffcolumns[8] ; /* buffers for messages */
    for(i=0 ; i < 8 ; i++) {
        buffcolumns[i] = (double*) malloc(max(process->automata->height,process->automata->width)*sizeof(double)) ;
        assert(buffcolumns[i]) ;
    }
    while(1) {
        /* Process ONLY reads stdin */
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
        /* Sending of the read values. */
        MPI_Bcast(buffio, 3, MPI_INT, ONLY, MPI_COMM_WORLD) ;
        MPI_Bcast(&content, 1, MPI_DOUBLE, ONLY, MPI_COMM_WORLD) ;
        if(buffio[0] == -1) /* EOF, exit */
            break ;
        else if(buffio[0] == 0) { /* new value */
            setCellValue(process->automata,buffio[1],buffio[2],content) ;
        }
        else if(buffio[0] == 1) { /* new constant */
            setCellConstant(process->automata,buffio[1],buffio[2],content) ;
        }
        else if(buffio[0] == 2) { /* new request */
            while(process->currentIter < process->nbIter) { /* computation of the delta function, if necessary */
                compute(process, buffcolumns) ;
            }
            content = getCell(process->automata,buffio[1],buffio[2]) ;
            if(!isnan(content) && !process->myid == ONLY) {
                MPI_Send(&content, 1, MPI_DOUBLE, ONLY, TAG, MPI_COMM_WORLD);
            }
            if(isnan(content) && process->myid == ONLY) {
                MPI_Recv(&content, 1, MPI_DOUBLE, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
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
    char filename[100] = "average.log." ;
    FILE *f ;
    int width, height, myid, nbproc, nbiter ;
    double p ;
    Process process ;
    MPI_Comm_rank(MPI_COMM_WORLD,&myid) ;
    MPI_Comm_size(MPI_COMM_WORLD, &nbproc) ;
    /* Process ONLY reads stdin */
    if(myid==ONLY) {
        scanf("%d",&width) ;
        scanf("%d",&height) ;
        scanf("%lf",&p) ;
        scanf("%d",&nbiter) ;
    }
    /* Sending of the read values */
    MPI_Bcast(&width, 1, MPI_INT, ONLY, MPI_COMM_WORLD) ;
    MPI_Bcast(&height, 1, MPI_INT, ONLY, MPI_COMM_WORLD) ;
    MPI_Bcast(&p, 1, MPI_DOUBLE, ONLY, MPI_COMM_WORLD) ;
    MPI_Bcast(&nbiter, 1, MPI_INT, ONLY, MPI_COMM_WORLD) ;
    /* MPI programs start with MPI_Init; all 'N' processes exist thereafter */
    process = initProcess(myid,nbproc,width,height,p,nbiter) ;
    if(process) { /* there is something to compute */
        int ok = 1 ;
        if(argc == 2 && !strcmp("-log",argv[1])) { /* logging process information */
            sprintf(&filename[sizeof("average.log.")-1], "%d", myid) ;
            f = fopen(filename,"w") ;
            printProcess(process,f) ;
            fclose(f) ;
        }
        else if(argc != 1) { /* wrong syntax */
            if(process->myid == ONLY) {
                printf("Syntax: ./%s\n",argv[0]) ;
                printf("        ./%s -log\n",argv[0]) ;
            }
            ok = 0;
        }
        if(ok) { /* computation */
            inputOutput(process) ;
            delProcess(process) ;
        }
    }
    MPI_Finalize() ;
    return 0 ;
}
