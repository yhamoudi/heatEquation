#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include "data.h"

#define max(a,b) (a>b ? a : b)
#define min(a,b) (a>b ? b : a)

AverageAutomata initAutomata(int width, int height, double p) {
    int i,j ;
    AverageAutomata automata = (AverageAutomata) malloc(sizeof(struct averageautomata)) ;
    automata->height = height ;
    automata->width = width ;
    automata->p = p ;
    automata->cells = (double**) malloc(height*sizeof(double*)) ;
    for(i=0 ; i < height ; i++) {
        automata->cells[i] = (double*) malloc(width*sizeof(double)) ;
        for(j = 0 ; j < width ; j++) {
            automata->cells[i][j] = 0 ;
        }
    }
    return automata ;
}

void delAutomata(AverageAutomata automata) {
    int i ;
    for(i=0 ; i < automata->height ; i++) {
        free(automata->cells[i]) ;
    }
    free(automata->cells) ;
    free(automata) ;
}

Process initProcess(int myid, int width, int height, double p) {
    Process process = (Process) malloc(sizeof(struct process)) ;
    MPI_Comm_size(MPI_COMM_WORLD, &process->nbproc);
    int a = (int) sqrt((double)(process->nbproc)) ;
    int b ;
    process->myid = myid ;
    /* Compute the width and the height of the grid of processes. */
    while(process->nbproc % a != 0)
        a-- ;
    b = process->nbproc/a ;
    if(width > height) {
        process->gridWidth = max(a,b) ;
        process->gridHeight = min(a,b) ;
    }
    else {
        process->gridWidth = min(a,b) ;
        process->gridHeight = max(a,b) ;
    }
    process->automata = initAutomata(0,0,p) ;
    return process ;
}

void delProcess(Process process) {
    delAutomata(process->automata) ;
    free(process) ;
}
