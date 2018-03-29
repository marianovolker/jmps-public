CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-pthread
SOURCES=pipes.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=example

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm *~ $(EXECUTABLE) *.o
	

