CC = g++
CFLAGS = -lSDL2
DEPS = /include/*
OBJECTS = /src/NES.o /src/BUS.o /src/PPU.o /src/ROM.o /src/input.o /src/GUI.o /src/CPU.o /src/main.o


all: 
	+$(MAKE) -C src

clean:
	+$(MAKE) clean -C src
