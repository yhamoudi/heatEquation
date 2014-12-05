#ifndef __DATA_H
#define __DATA_H

struct averageautomata {
    int height, width ;
    int p ;
    double **cells ;
};

typedef struct averageautomata *AverageAutomata;

/* Initialisation of the automata with the given height, width and factor p. */
AverageAutomata init(int height, int width, int p) ;


#endif
