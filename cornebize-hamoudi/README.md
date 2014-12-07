# Distributing the Heat Equation

## Compilation

```
make
```

## Execution

```
mpirun -n <number_processes> average < <file>
```

You can also produce log files for each process:
```
mpirun -n <number_processes> average -log < <file>
```
Each log file will have the name `average.log.i` where `i` is the index of the process.

Syntax of a log file:
```
## AUTOMATA ##
(height,width):                (<height of the local automata>, <width of the local automata>)
(heightOffset,widthOffset):    (<height offset of the local automata>, <width offset of the local automata>)

## PROCESS ##
(nbproc,gridHeight,gridWidth): (<number of working processes>, <height of the processes grid>, <width of the processes grid>)
(myid,myrow,mycol):            (<index of the process>, <column of the process in the grid>, <row of the process in the grid>)
(left,right,up,down):          (<index of the left process>, <index of the right process>, <index of the up process>, <index of the down process>)
```


## Annex scripts

Implementation of the average automata.

```
python3 oracle.py < <file>
```

A random average automaton generator.

```
python3 generator.py <width> <height> <p> <t>
```

A script to run automatically `average` for several number of processes.

```
./check.sh <file>
```
