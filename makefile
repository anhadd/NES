CC = g++
CFLAGS = -lSDL2
DEPS = /include/*
OBJECTS = /src/CPU.o /src/main.o


all: 
	+$(MAKE) -C src

clean:
	+$(MAKE) clean -C src
