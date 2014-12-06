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

/* Conversion from i,j coordinates to id. */
int coord_to_id(int i, int j, int width) {
    return i*width+j ;
}

/* True math modulo ( -1%5 == -1 in C) */
/* We suppose that -mod <= n <= 2mod */
int modulo(int n, int mod) {
    if(n<0)
        return n+mod ;
    if(n>=mod)
        return n-mod ;
    return n ;
}

/*
    Orientation of the processes grid:
                    top (i-1,j)
    left (i,j-1)     p (i,j)      right (i,j+1)
                    down (i+1,j)
*/
Process initProcess(int myid, int nbproc, int width, int height, double p) {
    Process process = (Process) malloc(sizeof(struct process)) ;
    int b ;
    process->myid = myid ;
    process->nbproc = nbproc ;
    int a = (int) sqrt((double)(process->nbproc)) ;
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
    process->myrow = process->myid/process->gridHeight ;
    process->mycol = process->myid%process->gridWidth ;
    process->left = coord_to_id(process->myrow,modulo(process->mycol-1,process->gridWidth),process->gridWidth) ;
    process->right = coord_to_id(process->myrow,modulo(process->mycol+1,process->gridWidth),process->gridWidth) ;
    process->down = coord_to_id(modulo(process->myrow+1,process->gridWidth),process->mycol,process->gridWidth) ;
    process->up = coord_to_id(modulo(process->myrow-1,process->gridWidth),process->mycol,process->gridWidth) ;
    process->automata = initAutomata(0,0,p) ;
    return process ;
}

void delProcess(Process process) {
    delAutomata(process->automata) ;
    free(process) ;
}
