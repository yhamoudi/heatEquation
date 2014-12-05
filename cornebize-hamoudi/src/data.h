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

/* Initialisation of the process with the given id, height, width and factor p (values for the global automata). */
Process initProcess(int myid, int nbproc, int width, int height, double p) ;

/* Deletetion of the process. */
void delProcess(Process process) ;


#endif
