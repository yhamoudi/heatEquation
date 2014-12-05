#ifndef __DATA_H
#define __DATA_H

struct averageautomata {
    int height, width ;
    double p ;
    double **cells ;
};

typedef struct averageautomata *AverageAutomata ;

/* Initialisation of the automata with the given height, width and factor p. */
AverageAutomata initAutomata(int height, int width, double p) ;

/* Deletion of the automata. */
void delAutomata(AverageAutomata automata) ;


#endif
