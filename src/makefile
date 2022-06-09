# Use g++.
CXX = g++
# -std=c++11 sets the C++ version to 11.
CXXFLAGS = -std=c++11
# Link with SDL2.
CFLAGS = -lSDL2
# The dependencies is everything in the include folder (header files).
DEPS = /include/*
# The objects that need to be created. One for every cpp file.
OBJECTS = Mapper3.o Mapper2.o Mapper1.o Mapper0.o Mapper.o NES.o BUS.o PPU.o ROM.o input.o GUI.o CPU.o main.o

# Define what 'make all' does.
all: ../main

# Rule for creating the main binary.
../main: $(OBJECTS)
	$(CXX) -o $@ $^ $(CFLAGS)

# Rule for creating the .o (Object) files.
%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

# Define what 'make clean' does.
clean:
	rm -f *.o NES
	rm -f ../main NES
