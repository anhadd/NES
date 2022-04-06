CC = g++
CFLAGS = -lSDL2
DEPS = /include/*
OBJECTS = NES.o input.o GUI.o SFX.o main.o


all: ../main

../main: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o NES
	rm -f ../main NES
