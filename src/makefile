CXX = g++
CXXFLAGS = -std=c++11
CFLAGS = -lSDL2
DEPS = /include/*
OBJECTS = Mapper3.o Mapper2.o Mapper1.o Mapper0.o Mapper.o NES.o BUS.o APU.o PPU.o ROM.o input.o GUI.o CPU.o main.o


all: ../main

../main: $(OBJECTS)
	$(CXX) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o NES
	rm -f ../main NES
