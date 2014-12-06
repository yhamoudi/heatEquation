#include <assert.h>
#include <stdio.h>
#include "data.h"

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

void checkProcess(int myid, int nbproc, int globalwidth, int globalheight, double p,\
                 int gridWidth, int gridHeight, int myrow, int mycol, int left, int right, int up, int down,\
                 int width, int height, int widthOffset, int heightOffset) {
    Process proc = initProcess(myid,nbproc,globalwidth,globalheight,p) ;
    assert(proc->gridWidth == gridWidth) ;
    assert(proc->gridHeight == gridHeight) ;
    assert(proc->myrow == myrow) ;
    assert(proc->mycol == mycol) ;
    assert(proc->left == left) ;
    assert(proc->right == right) ;
    assert(proc->up == up) ;
    assert(proc->down == down) ;
    checkAutomata(proc->automata,width,height,widthOffset,heightOffset,p) ;
    delProcess(proc) ;
}

void testMiddleProcess() {
    nbtest ++ ;
    checkProcess(5,12,11,8,0.1,\
                 4,3,1,1,4,6,1,9,\
                 3,3,3,3) ;
}

void testLeftEdgeProcess() {
    nbtest ++ ;
    checkProcess(5,12,11,8,0.1,\
                 4,3,1,1,4,6,1,9,\
                 3,3,3,3) ;
}

void testRightEdgeProcess() {
    nbtest ++ ;
    checkProcess(7,12,11,8,0.1,\
                 4,3,1,3,6,4,3,11,\
                 2,3,9,3) ;
}

void testTopEdgeProcess() {
    nbtest ++ ;
    checkProcess(1,12,11,8,0.1,\
                 4,3,0,1,0,2,9,5,\
                 3,3,3,0) ;
}

void testDownEdgeProcess() {
    nbtest ++ ;
    checkProcess(9,12,11,8,0.1,\
                 4,3,2,1,8,10,5,1,\
                 3,2,3,6) ;
}

int main() {
    testMiddleProcess() ;
    testLeftEdgeProcess() ;
    testRightEdgeProcess() ;
    testTopEdgeProcess() ;
    testDownEdgeProcess() ;
    printf("Passed %d tests successfully.\n",nbtest);
    return 0 ;
}
