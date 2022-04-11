CXX = g++
CXXFLAGS = -std=c++11
CFLAGS = -lSDL2
DEPS = /include/*
OBJECTS = ROM.o input.o GUI.o CPU.o main.o


all: ../main

../main: $(OBJECTS)
	$(CXX) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o NES
	rm -f ../main NES
