#ifndef __DATA_H
#define __DATA_H

typedef struct{
    double content;
    enum type {VALUE,CONSTANT} type;
}cell;

struct averageautomata {
    int height, width ;
    int widthOffset, heightOffset ;
    double p ;
    cell **cells ;
};

typedef struct averageautomata *AverageAutomata ;

/* Initialisation of the automata with the given height, width, offsets and factor p. */
AverageAutomata initAutomata(int width, int height, int widthOffset, int heightOffset, double p) ;

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
