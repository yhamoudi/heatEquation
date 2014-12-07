#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "automata.h"

AverageAutomata initAutomata(int width, int height, int widthOffset, int heightOffset, double p) {
    int i,j ;
    AverageAutomata automata = (AverageAutomata) malloc(sizeof(struct averageautomata)) ;
    assert(automata) ;
    automata->height = height ;
    automata->width = width ;
    automata->widthOffset = widthOffset ;
    automata->heightOffset = heightOffset ;
    automata->p = p ;
    automata->cells = (cell**) malloc((height+2)*sizeof(cell*)) ;
    assert(automata->cells) ;
    for(i=0 ; i < height+2 ; i++) {
        automata->cells[i] = (cell*) malloc((width+2)*sizeof(cell)) ;
        assert(automata->cells[i]) ;
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

void printAutomata(AverageAutomata automata, FILE *f) {
    fprintf(f,"## AUTOMATA ##\n") ;
    fprintf(f,"(height,width):                (%d\t,%d)\n",automata->height,automata->width) ;
    fprintf(f,"(heightOffset,widthOffset):    (%d\t,%d)\n",automata->heightOffset,automata->widthOffset) ;
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

cell **getnewcells(AverageAutomata automata) {
    int i,j ;
    cell **newcells = (cell**) malloc((automata->height+2)*sizeof(cell*)) ;
    assert(newcells) ;
    for(i=0 ; i < automata->height+2 ; i++) {
        newcells[i] = (cell*) malloc((automata->width+2)*sizeof(cell)) ;
        assert(newcells[i]) ;
        for(j = 0 ; j < automata->width+2 ; j++) {
            newcells[i][j].content = automata->cells[i][j].content ;
            newcells[i][j].type = automata->cells[i][j].type ;
        }
    }
    return newcells ;
}

void delta(AverageAutomata automata) {
    int i, j ;
    cell **newcells ;
    newcells = getnewcells(automata) ;
    for(i=1 ; i <= automata->height ; i++) {
        for(j = 1 ; j <= automata->width ; j++) {
            if(automata->cells[i][j].type == VALUE) {
                newcells[i][j].content = (1-automata->p)*automata->cells[i][j].content +\
                    automata->p*(automata->cells[i-1][j].content+automata->cells[i+1][j].content+automata->cells[i][j-1].content+automata->cells[i][j+1].content)/4 ;
            }
        }
    }
    for(i=0 ; i < automata->height+2 ; i++) {
        free(automata->cells[i]) ;
    }
    free(automata->cells) ;
    automata->cells = newcells ;
}
