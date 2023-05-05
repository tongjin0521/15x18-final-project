#!/bin/bash

# Check if filename is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <filename>"
  exit 1
fi

# Save current stdout location to a file descriptor
exec 3>&1
# Redirect stdout to the specified file
exec > "$1"

./gwo-sequential 5 100 cleavland.csv
mpirun -n 5 gwo-MPI 5 100 cleavland.csv
./gwo-OpenMP 5 100 cleavland.csv

# Restore stdout to the original location
exec 1>&3
# Close the file descriptor
exec 3>&-
