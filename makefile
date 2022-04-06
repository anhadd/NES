CC = g++
CFLAGS = -lSDL2
DEPS = /include/*
OBJECTS = /src/NES.o /src/input.o /src/GUI.o /src/main.o


all: 
	+$(MAKE) -C src

clean:
	+$(MAKE) clean -C src
