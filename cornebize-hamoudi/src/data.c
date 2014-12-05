#include <stdlib.h>
#include <stdio.h>
#include "data.h"

AverageAutomata initAutomata(int height, int width, double p) {
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
