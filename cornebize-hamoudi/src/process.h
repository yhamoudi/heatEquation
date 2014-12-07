#ifndef __PROCESS_H
#define __PROCESS_H

#include "automata.h"

struct process {
    int nbproc, gridHeight, gridWidth;
    int myid, myrow, mycol;
    int left, right, up, down ;
    int currentIter, nbIter ;
    AverageAutomata automata ;
};

typedef struct process *Process ;

/* Compute all the necessary informations for the process. */
/* Returns NULL if the process must be killed (it has nothing to compute. */
Process initProcess(int myid, int nbproc, int width, int height, double p, int nbIter) ;

/* Deletetion of the process. */
void delProcess(Process process) ;

void printProcess(Process process, FILE *f) ;

#endif
