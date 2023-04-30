# Compiler to use for sequential version
CC=g++

# Compiler to use for MPI version
MPIC = mpic++

# Compiler flags
CFLAGS=-std=c++14 -fvisibility=hidden -lpthread -Wall -Wextra -O2

# Source files for sequential version
SEQUENTIAL_SOURCES=src/gwo-sequential.cpp src/common.cpp

# Source files for MPI version
MPI_SOURCES=src/gwo-MPI.cpp src/common.cpp

# Object files for sequential version
SEQUENTIAL_OBJECTS=$(SEQUENTIAL_SOURCES:.cpp=.o)

# Object files for MPI version
MPI_OBJECTS=$(MPI_SOURCES:.cpp=.o)

# Executable names
SEQUENTIAL_EXECUTABLE=gwo-sequential
MPI_EXECUTABLE=gwo-MPI

all: sequential mpi

# Compile and link the sequential version
sequential: $(SEQUENTIAL_EXECUTABLE)

$(SEQUENTIAL_EXECUTABLE): $(SEQUENTIAL_OBJECTS)
	$(CC) $(SEQUENTIAL_OBJECTS) -o $@

# Compile and link the MPI version
mpi: $(MPI_EXECUTABLE)

$(MPI_EXECUTABLE): $(MPI_OBJECTS)
	$(MPIC) $(MPI_OBJECTS) -o $@

# Compile the common source file into an object file
src/common.o: src/common.cpp src/common.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the sequential version source files into object files
src/gwo-sequential.o: src/gwo-sequential.cpp src/common.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the MPI version source files into object files
src/gwo-MPI.o: src/gwo-MPI.cpp src/common.h
	$(MPIC) $(CFLAGS) -c $< -o $@

# Remove object files and executables
clean:
	rm -f $(SEQUENTIAL_OBJECTS) $(MPI_OBJECTS) $(SEQUENTIAL_EXECUTABLE) $(MPI_EXECUTABLE)
