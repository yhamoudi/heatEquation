#include <stdlib.h>
#include <stdio.h>
#include "data.h"

AverageAutomata init(int height, int width, int p) {
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
