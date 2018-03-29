CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lrt -pthread

SOURCES=posix_anonymous_shm_and_named_posix_sem.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=posix_anonymous_shm_and_named_posix_sem

SOURCES_SHM_CLIENT=shm_msgclient.c
OBJECTS_SHM_CLIENT=$(SOURCES_SHM_CLIENT:.c=.o)
EXEC_SHM_CLIENT=shm_msgclient

SOURCES_SHM_SERVER=shm_msgserver.c
OBJECTS_SHM_SERVER=$(SOURCES_SHM_SERVER:.c=.o)
EXEC_SHM_SERVER=shm_msgserver

BINARIES=$(EXECUTABLE) $(EXEC_SHM_SERVER) $(EXEC_SHM_CLIENT)

all: $(SOURCES) $(SOURCES_SHM_SERVER) $(SOURCES_SHM_CLIENT) $(BINARIES)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(EXEC_SHM_SERVER): $(OBJECTS_SHM_SERVER)
	$(CC) $(OBJECTS_SHM_SERVER) -o $@ $(LDFLAGS)

$(EXEC_SHM_CLIENT): $(OBJECTS_SHM_CLIENT)
	$(CC) $(OBJECTS_SHM_CLIENT) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm *~ $(BINARIES) *.o
	

