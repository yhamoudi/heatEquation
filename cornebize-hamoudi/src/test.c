#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "automata.h"
#include "process.h"

/* Test file without MPI. */

int nbtest = 0 ;

void checkAutomata(AverageAutomata automata, int width, int height, int widthOffset, int heightOffset, double p) {
    int i,j ;
    assert(automata->p == p) ;
    assert(automata->width == width) ;
    assert(automata->height == height) ;
    assert(automata->widthOffset == widthOffset) ;
    assert(automata->heightOffset == heightOffset) ;
    for(i=0 ; i < automata->height+2 ; i++) {
        for(j=0 ; j < automata->width+2 ; j++) {
            assert(automata->cells[i][j].content == 0) ;
            assert(automata->cells[i][j].type == VALUE) ;
        }
    }
}

void checkProcess(int myid, int nbproc, int globalwidth, int globalheight, double p, int nbIter,\
                 int gridWidth, int gridHeight, int myrow, int mycol, int left, int right, int up, int down,\
                 int width, int height, int widthOffset, int heightOffset) {
    Process proc = initProcess(myid,nbproc,globalwidth,globalheight,p,nbIter) ;
    assert(proc->gridWidth == gridWidth) ;
    assert(proc->gridHeight == gridHeight) ;
    assert(proc->myrow == myrow) ;
    assert(proc->mycol == mycol) ;
    assert(proc->left == left) ;
    assert(proc->right == right) ;
    assert(proc->up == up) ;
    assert(proc->down == down) ;
    assert(proc->nbIter == nbIter) ;
    assert(proc->currentIter == 0) ;
    checkAutomata(proc->automata,width,height,widthOffset,heightOffset,p) ;
    delProcess(proc) ;
}

void testMiddleProcess() {
    nbtest ++ ;
    checkProcess(5,12,11,8,0.1,42,\
                 4,3,1,1,4,6,1,9,\
                 3,3,3,3) ;
}

void testLeftEdgeProcess() {
    nbtest ++ ;
    checkProcess(5,12,11,8,0.1,42,\
                 4,3,1,1,4,6,1,9,\
                 3,3,3,3) ;
}

void testRightEdgeProcess() {
    nbtest ++ ;
    checkProcess(7,12,11,8,0.1,42,\
                 4,3,1,3,6,4,3,11,\
                 2,3,9,3) ;
}

void testTopEdgeProcess() {
    nbtest ++ ;
    checkProcess(1,12,11,8,0.1,42,\
                 4,3,0,1,0,2,9,5,\
                 3,3,3,0) ;
}

void testDownEdgeProcess() {
    nbtest ++ ;
    checkProcess(9,12,11,8,0.1,42,\
                 4,3,2,1,8,10,5,1,\
                 3,2,3,6) ;
}

void testIdleProcess() {
    nbtest ++ ;
    Process proc = initProcess(4,5,11,8,0.1,42) ;
    assert(!proc) ;
}

void testRightEdgeIdleProcess() {
    nbtest ++ ;
    checkProcess(3,5,11,8,0.1,42,\
                 4,1,0,3,2,0,3,3,\
                 2,8,9,0) ;
}

void testLeftEdgeIdleProcess() {
    nbtest ++ ;
    checkProcess(0,5,11,8,0.1,42,\
                 4,1,0,0,3,1,0,0,\
                 3,8,0,0) ;
}

void checkSetGet() {
    nbtest ++ ;
    Process proc = initProcess(5,12,11,8,0.1,10) ;
    assert(setCellValue(proc->automata,1,2,0.25) == 0) ;
    assert(setCellValue(proc->automata,10,7,0.25) == 0) ;
    assert(setCellValue(proc->automata,4,4,0.25) == 1) ;
    assert(getCell(proc->automata,3,5) == 0) ;
    assert(getCell(proc->automata,4,4) == 0.25) ;
    assert(isnan(getCell(proc->automata,1,2))) ;
    assert(isnan(getCell(proc->automata,10,7))) ;
    assert(proc->automata->cells[2][2].content == 0.25) ;
    delProcess(proc) ;
}

void checkDelta() {
    nbtest ++ ;
    Process proc = initProcess(5,12,11,8,0.25,10) ;
    proc->automata->cells[1][2].content = 1 ;
    proc->automata->cells[2][2].content = 1 ;
    proc->automata->cells[2][1].content = 1 ;
    proc->automata->cells[2][3].content = 1 ;
    proc->automata->cells[3][2].content = 1 ;
    delta(proc->automata) ;
    assert(proc->automata->cells[2][2].content == 1) ;
    assert(proc->automata->cells[1][2].content == 0.8125) ;
    assert(proc->automata->cells[2][1].content == 0.8125) ;
    assert(proc->automata->cells[2][3].content == 0.8125) ;
    assert(proc->automata->cells[3][2].content == 0.8125) ;
    assert(proc->automata->cells[1][1].content == 0.125) ;
    assert(proc->automata->cells[1][3].content == 0.125) ;
    assert(proc->automata->cells[3][1].content == 0.125) ;
    assert(proc->automata->cells[3][3].content == 0.125) ;
    delProcess(proc) ;
}

int main() {
    testMiddleProcess() ;
    testLeftEdgeProcess() ;
    testRightEdgeProcess() ;
    testTopEdgeProcess() ;
    testDownEdgeProcess() ;
    testIdleProcess() ;
    testRightEdgeIdleProcess() ;
    testLeftEdgeIdleProcess() ;
    checkSetGet() ;
    checkDelta() ;
    printf("Passed %d tests successfully.\n",nbtest);
    return 0 ;
}
