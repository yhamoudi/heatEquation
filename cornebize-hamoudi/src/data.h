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

/* Set the given cell to a value, with the given content. */
/* Note that the coordinates are global coordinates. If the cell does not belong to the automata, then nothing is done. */
/* If the cell was a constant, does not change it. */
/* Return 1 if a change was made, 0 otherwise. */
int setCellValue(AverageAutomata automata, int i, int j, double content) ;

/* Set the given cell to a constant, with the given content. */
/* Note that the coordinates are global coordinates. If the cell does not belong to the automata, then nothing is done. */
/* If the cell was a value, then it change it to a constant. */
/* Return 1 if a change was made, 0 otherwise. */
int setCellConstant(AverageAutomata automata, int i, int j, double content) ;

/* Get the content of the given cell. */
/* Note that the coordinates are global coordinates. If the cell does not belong to the automata, then NaN is returned. */
double getCell(AverageAutomata automata, int i, int j) ;

/* Compute one step of the delta function. */
void delta(AverageAutomata automata) ;

struct process {
    int nbproc, gridHeight, gridWidth;
    int myid, myrow, mycol;
    int left, right, up, down ;
    int currentIter, nbIter ;
    AverageAutomata automata ;
};

typedef struct process *Process ;

/* Initialisation of the process with the given id, height, width and factor p (values for the global automata). */
Process initProcess(int myid, int nbproc, int width, int height, double p, int nbIter) ;

/* Deletetion of the process. */
void delProcess(Process process) ;


#endif
