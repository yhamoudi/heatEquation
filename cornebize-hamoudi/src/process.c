#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <assert.h>
#include "process.h"

#define max(a,b) (a>b ? a : b)
#define min(a,b) (a>b ? b : a)

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
Process initProcess(int myid, int nbproc, int width, int height, double p, int nbIter) {
    Process process = (Process) malloc(sizeof(struct process)) ;
    assert(process) ;
    int cellsWidth, cellsHeight, cellsWidthOffset, cellsHeightOffset ;
    process->myid = myid ;
    process->nbproc = nbproc ;
    int a = (int) floor(sqrt((double)(process->nbproc))), b ;
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
    /* Informations about positionning in the grid of processes. */
    process->nbIter = nbIter ;
    process->currentIter = 0 ;
    /* Default size of the handled automata. */
    cellsWidth = (int)ceil((double)width/(double)process->gridWidth) ;
    cellsHeight = (int)ceil((double)height/(double)process->gridHeight) ;
    /* Redimmensionning of the grid of processes (removing idle processes). */
    while((process->gridWidth-1)*cellsWidth >= width)
        process->gridWidth -- ;
    while((process->gridHeight-1)*cellsHeight >= height)
        process->gridHeight -- ;
    process->nbproc = process->gridWidth*process->gridHeight ;
    if(process->myid >= process->nbproc) { /* will be killed */
        free(process) ;
        return NULL ;
    }
    process->myrow = process->myid/process->gridWidth ;
    process->mycol = process->myid%process->gridWidth ;
    /* Left, right, top and down processes. */
    process->left = coord_to_id(process->myrow,modulo(process->mycol-1,process->gridWidth),process->gridWidth) ;
    process->right = coord_to_id(process->myrow,modulo(process->mycol+1,process->gridWidth),process->gridWidth) ;
    process->down = coord_to_id(modulo(process->myrow+1,process->gridHeight),process->mycol,process->gridWidth) ;
    process->up = coord_to_id(modulo(process->myrow-1,process->gridHeight),process->mycol,process->gridWidth) ;
    /* Informations about the part of the automata to handle. */
    cellsWidthOffset = cellsWidth*process->mycol ;
    cellsHeightOffset = cellsHeight*process->myrow ;
    cellsWidth = max(0,min(cellsWidth,width-cellsWidthOffset)) ;
    cellsHeight = max(0,min(cellsHeight,height-cellsHeightOffset)) ;
    process->automata = initAutomata(cellsWidth,cellsHeight,cellsWidthOffset,cellsHeightOffset,p) ;
    return process ;
}

void delProcess(Process process) {
    delAutomata(process->automata) ;
    free(process) ;
}

void printProcess(Process process, FILE *f) {
    printAutomata(process->automata, f) ;
    fprintf(f,"\n## PROCESS ##\n") ;
    fprintf(f,"(nbproc,gridHeight,gridWidth): (%d\t,%d\t,%d)\n",process->nbproc,process->gridHeight,process->gridWidth) ;
    fprintf(f,"(myid,myrow,mycol):            (%d\t,%d\t,%d)\n",process->myid,process->myrow,process->mycol) ;
    fprintf(f,"(left,right,up,down):          (%d\t,%d\t,%d\t,%d)\n",process->left,process->right,process->up,process->down) ;
}
