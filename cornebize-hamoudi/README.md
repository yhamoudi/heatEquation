# Distributing the Heat Equation

## Compilation

```
make
```

## Execution

```
mpirun -n <number_processes> average < <file>
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
