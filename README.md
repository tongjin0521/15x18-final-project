# 15x18-final-project

Compile with `make`.

Run the sequential version with `./gwo-sequential`.

Run the MPI version with 
``` bash
mpirun -n 5 gwo-MPI 
```

Run the OpenMP version with `./gwo-OpenMP`.

All versions may have up to two command-line arguments: the first one is the number of agents (defaults to 5) and the second one is the number of iterations (defaults to 10). If only one argument is given, it is interpreted as the number of agents. 
