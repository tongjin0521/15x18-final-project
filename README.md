# 15x18-final-project

Compile with `make`. If on psc, remember to run `module load openmpi/4.0.2-gcc8.3.1` first.

Run the sequential version with `./gwo-sequential`.

Run the MPI version with the following command (remember to run `interact -p RM` if on psc):
``` bash
mpirun -n 5 gwo-MPI 
```

Run the OpenMP version with `./gwo-OpenMP`.

All versions may have up to three command-line arguments: 
1. the number of agents (defaults to 5).
2. the number of iterations (defaults to 10). 
3. the data source as a string (defaults to "cleavland.csv").

If fewer arguments are provided, they will be filled in from the right. For example, if only the number of agents is provided, the number of iterations will be 10 and the data source will be "cleavland.csv".
