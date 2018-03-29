CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-pthread

SOURCES_1=shm_server.c
OBJECTS_1=$(SOURCES_1:.c=.o)
EXEC_1=shm_server

SOURCES_2=shm_client.c
OBJECTS_2=$(SOURCES_2:.c=.o)
EXEC_2=shm_client


BINARIES=$(EXEC_1) $(EXEC_2)

all: $(SOURCES_1) $(SOURCES_2) $(BINARIES)



$(EXEC_1): $(OBJECTS_1)
	$(CC) $(LDFLAGS) $(OBJECTS_1) -o $@

$(EXEC_2): $(OBJECTS_2)
	$(CC) $(LDFLAGS) $(OBJECTS_2) -o $@


.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm *~ $(BINARIES) *.o
	

