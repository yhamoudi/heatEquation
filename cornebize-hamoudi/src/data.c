#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include "data.h"

#define max(a,b) (a>b ? a : b)
#define min(a,b) (a>b ? b : a)

AverageAutomata initAutomata(int width, int height, int widthOffset, int heightOffset, double p) {
    int i,j ;
    AverageAutomata automata = (AverageAutomata) malloc(sizeof(struct averageautomata)) ;
    automata->height = height ;
    automata->width = width ;
    automata->widthOffset = widthOffset ;
    automata->heightOffset = heightOffset ;
    automata->p = p ;
    automata->cells = (cell**) malloc((height+2)*sizeof(cell*)) ;
    for(i=0 ; i < height+2 ; i++) {
        automata->cells[i] = (cell*) malloc((width+2)*sizeof(cell)) ;
        for(j = 0 ; j < width+2 ; j++) {
            automata->cells[i][j].content = 0 ;
            automata->cells[i][j].type = VALUE ;
        }
    }
    return automata ;
}

void delAutomata(AverageAutomata automata) {
    int i ;
    for(i=0 ; i < automata->height+2 ; i++) {
        free(automata->cells[i]) ;
    }
    free(automata->cells) ;
    free(automata) ;
}

int setCellValue(AverageAutomata automata, int i, int j, double content) {
    if(i >= automata->heightOffset && j >= automata->widthOffset && i < automata->heightOffset + automata->height && j < automata->widthOffset + automata->width) {
        if(automata->cells[i-automata->heightOffset+1][j-automata->widthOffset+1].type == CONSTANT)
            return 0 ;
        automata->cells[i-automata->heightOffset+1][j-automata->widthOffset+1].content = content ;
        return 1 ;
    }
    return 0 ;
}

int setCellConstant(AverageAutomata automata, int i, int j, double content) {
    if(i >= automata->heightOffset && j >= automata->widthOffset && i < automata->heightOffset + automata->height && j < automata->widthOffset + automata->width) {
        automata->cells[i-automata->heightOffset+1][j-automata->widthOffset+1].content = content ;
        automata->cells[i-automata->heightOffset+1][j-automata->widthOffset+1].type = CONSTANT ;
        return 1 ;
    }
    return 0 ;
}

double getCell(AverageAutomata automata, int i, int j) {
    if(i >= automata->heightOffset && j >= automata->widthOffset && i < automata->heightOffset + automata->height && j < automata->widthOffset + automata->width) {
        return automata->cells[i-automata->heightOffset+1][j-automata->widthOffset+1].content ;
    }
    return NAN ;
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
Process initProcess(int myid, int nbproc, int width, int height, double p, int nbIter) {
    Process process = (Process) malloc(sizeof(struct process)) ;
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
    process->myrow = process->myid/process->gridWidth ;
    process->mycol = process->myid%process->gridWidth ;
    process->left = coord_to_id(process->myrow,modulo(process->mycol-1,process->gridWidth),process->gridWidth) ;
    process->right = coord_to_id(process->myrow,modulo(process->mycol+1,process->gridWidth),process->gridWidth) ;
    process->down = coord_to_id(modulo(process->myrow+1,process->gridHeight),process->mycol,process->gridWidth) ;
    process->up = coord_to_id(modulo(process->myrow-1,process->gridHeight),process->mycol,process->gridWidth) ;
    /* Informations about the part of the automata to handle. */
    cellsWidth = (int)ceil((double)width/(double)process->gridWidth) ;
    cellsHeight = (int)ceil((double)height/(double)process->gridHeight) ;
    cellsWidthOffset = cellsWidth*process->mycol ;
    cellsHeightOffset = cellsHeight*process->myrow ;
    cellsWidth = min(cellsWidth,width-cellsWidthOffset) ;
    cellsHeight = min(cellsHeight,height-cellsHeightOffset) ;
    process->automata = initAutomata(cellsWidth,cellsHeight,cellsWidthOffset,cellsHeightOffset,p) ;
    return process ;
}

void delProcess(Process process) {
    delAutomata(process->automata) ;
    free(process) ;
}
