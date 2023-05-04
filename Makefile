# Compiler to use for sequential version
CC=g++

# Compiler to use for MPI version
MPIC = mpic++

# Compiler to use for OPENMP version
OPENMPC = -fopenmp

# Compiler flags
CFLAGS=-std=c++14 -fvisibility=hidden -lpthread -Wall -Wextra -O2

# Source files for SVM
SVM_SOURCE=src/svm.cpp

# Source files for sequential version
SEQUENTIAL_SOURCES=src/gwo-sequential.cpp src/common.cpp $(SVM_SOURCE)

# Source files for MPI version
MPI_SOURCES=src/gwo-MPI.cpp src/common.cpp $(SVM_SOURCE)

# Source files for OpenMP version
OPENMP_SOURCES=src/gwo-OpenMP.cpp src/common.cpp $(SVM_SOURCE)

# Object files for sequential version
SEQUENTIAL_OBJECTS=$(SEQUENTIAL_SOURCES:.cpp=.o)

# Object files for MPI version
MPI_OBJECTS=$(MPI_SOURCES:.cpp=.o)

# Object files for MPI version
OPENMP_OBJECTS=$(OPENMP_SOURCES:.cpp=.o)

# Executable names
SEQUENTIAL_EXECUTABLE=gwo-sequential
MPI_EXECUTABLE=gwo-MPI
OPENMP_EXECUTABLE=gwo-OpenMP

all: sequential openmp

# Compile and link the sequential version
sequential: $(SEQUENTIAL_EXECUTABLE)

$(SEQUENTIAL_EXECUTABLE): $(SEQUENTIAL_OBJECTS)
	$(CC) $(SEQUENTIAL_OBJECTS) -o $@

# Compile and link the MPI version
mpi: $(MPI_EXECUTABLE)

$(MPI_EXECUTABLE): $(MPI_OBJECTS)
	$(MPIC) $(MPI_OBJECTS) -o $@

# Compile and link the OPENMP version
openmp: $(OPENMP_EXECUTABLE)

$(OPENMP_EXECUTABLE): $(OPENMP_OBJECTS)
	$(CC) $(CFLAGS) $(OPENMPC) $(OPENMP_OBJECTS) -o $@

# Compile the SVM source file into an object file
src/svm.o: src/svm.cpp src/svm.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the common source file into an object file
src/common.o: src/common.cpp src/common.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the sequential version source files into object files
src/gwo-sequential.o: src/gwo-sequential.cpp src/common.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the MPI version source files into object files
src/gwo-MPI.o: src/gwo-MPI.cpp src/common.h
	$(MPIC) $(CFLAGS) -c $< -o $@

# Compile the OpenMP version source files into object files
src/gwo-OpenMP.o: src/gwo-OpenMP.cpp src/common.h
	$(CC) $(CFLAGS) $(OPENMPC) -c $< -o $@


# Remove object files and executables
clean:
	rm -f $(SEQUENTIAL_OBJECTS) $(MPI_OBJECTS) $(OPENMP_OBJECTS) $(OPENMP_EXECUTABLE) $(SEQUENTIAL_EXECUTABLE) $(MPI_EXECUTABLE)
