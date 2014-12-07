# Distributing the Heat Equation

## Issues

Some issues were encountered, depending on the version of mpicc.

We tested the following command on several systems `mpirun -n 8 average < examples/1`:

It worked for the following versions of mpicc (in particular, it worked on servsls.ens-lyon.fr):

* gcc (Debian 4.8.2-1) 4.8.2
* gcc (Debian 4.7.2-5) 4.7.2

It produced a segmentation fault for the following version of mpicc:

* gcc (Ubuntu 4.8.2-19ubuntu1) 4.8.2

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


## Examples

Several examples are provided.

`examples/1` and `examples/2` are two basic automata.

`examples/one_to_heat_them_all` is a single constant cell with content 1. It will heat the whole automaton over time.

`examples/examples/thermal_reservoirs` show 4 thermal reservoirs: two with content 0, and two with content 1.
