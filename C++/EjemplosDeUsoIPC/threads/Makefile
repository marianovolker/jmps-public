CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-pthread

SOURCES_THREADS_1=simple_threads.c
OBJECTS_THREADS_1=$(SOURCES_THREADS_1:.c=.o)
EXEC_THREADS_1=simple_threads

SOURCES_THREADS_2=simple_mutex.c
OBJECTS_THREADS_2=$(SOURCES_THREADS_2:.c=.o)
EXEC_THREADS_2=simple_mutex


BINARIES=$(EXEC_THREADS_1) $(EXEC_THREADS_2)

all: $(SOURCES_THREADS_1) $(SOURCES_THREADS_2) $(BINARIES)



$(EXEC_THREADS_1): $(OBJECTS_THREADS_1)
	$(CC) $(LDFLAGS) $(OBJECTS_THREADS_1) -o $@

$(EXEC_THREADS_2): $(OBJECTS_THREADS_2)
	$(CC) $(LDFLAGS) $(OBJECTS_THREADS_2) -o $@


.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm *~ $(BINARIES) *.o
	

