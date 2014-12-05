#ifndef __DATA_H
#define __DATA_H

struct averageautomata {
    int height, width ;
    double p ;
    double **cells ;
};

typedef struct averageautomata *AverageAutomata ;

/* Initialisation of the automata with the given height, width and factor p. */
AverageAutomata initAutomata(int width, int height, double p) ;

/* Deletion of the automata. */
void delAutomata(AverageAutomata automata) ;


struct process {
    int nbproc, gridHeight, gridWidth;
    int myid, myrow, mycol;
    int left, right, up, down ;
    AverageAutomata automata ;
};

typedef struct process *Process ;

Process initProcess(int myid, int width, int height) ;

void delProcess(Process process) ;


#endif
