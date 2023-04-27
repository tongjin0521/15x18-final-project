# Compiler to use
CC=g++

# Compiler flags
CFLAGS=-c -Wall

# Source files
SOURCES=src/gwo-sequential.cpp src/common.cpp

# Object files
OBJECTS=$(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE=gwo-sequential

# Compile and link the executable
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

# Compile the source files into object files
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

# Remove object files and executable
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
