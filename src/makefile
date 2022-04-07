CC = g++
CFLAGS = -lSDL2
DEPS = /include/*
OBJECTS = CPU.o main.o


all: ../main

../main: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o NES
	rm -f ../main NES
