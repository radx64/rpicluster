CC=mpicc
CFLAGS=-c -Wall
LDFLAGS=-lm
SOURCES=main.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=simpleMath

.PHONY: all clean run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f $(EXECUTABLE)

run: $(EXECUTABLE)
	mpirun -np 4 $(EXECUTABLE)