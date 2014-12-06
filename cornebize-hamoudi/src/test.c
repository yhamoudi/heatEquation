#include <assert.h>
#include <stdio.h>
#include "data.h"

/* Test file without MPI. */
/*
    myid    = 8
    nbproc  = 12
    width   = 11
    height  = 9
    p       = 0.1
*/


int main() {
    Process p = initProcess(8,12,11,9,0.1) ;
    assert(p->gridWidth == 4) ;
    assert(p->gridHeight == 3) ;
    assert(p->myrow == 2) ;
    assert(p->mycol == 0) ;
    assert(p->left == 11) ;
    assert(p->right == 9) ;
    assert(p->up == 4) ;
    assert(p->down == 12) ;
    delProcess(p) ;
    return 0 ;
}
