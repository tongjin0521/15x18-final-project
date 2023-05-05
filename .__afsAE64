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

for ((n=4;n<=13;n++))
do
  ./gwo-sequential $n 10 cleavland.csv
  mpirun -n $n gwo-MPI $n 10 cleavland.csv
  ./gwo-OpenMP $n 10 cleavland.csv
done

for ((n=10;n<=100;n+=10))
do
  ./gwo-sequential 5 $n cleavland.csv
  mpirun -n 5 gwo-MPI 5 $n cleavland.csv
  ./gwo-OpenMP 5 $n cleavland.csv
done

csv_files=("cleavland-9.csv" "cleavland-100.csv" "cleavland-600.csv" "cleavland-1000.csv")

for csv_file in "${csv_files[@]}"
do
  ./gwo-sequential 5 10 "$csv_file"
  mpirun -n 5 gwo-MPI 5 10 "$csv_file"
  ./gwo-OpenMP 5 10 "$csv_file"
done

# Restore stdout to the original location
exec 1>&3
# Close the file descriptor
exec 3>&-
