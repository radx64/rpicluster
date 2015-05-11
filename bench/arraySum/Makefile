CC=mpicc
CFLAGS=-c
DFLAGS?=INT
LDFLAGS=-lm
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=arraySum

.PHONY: all clean run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -D$(DFLAGS) $< -o $@

clean:
	-rm -f $(EXECUTABLE)

run: $(EXECUTABLE)
	mpirun -np 4 ./$(EXECUTABLE)
