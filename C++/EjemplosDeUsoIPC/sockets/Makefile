CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-pthread

SOURCES_TCP_CLIENT=socket_tcp_client.c
OBJECTS_TCP_CLIENT=$(SOURCES_TCP_CLIENT:.c=.o)
EXEC_TCP_CLIENT=socket_tcp_client

SOURCES_TCP_SERVER=socket_tcp_server.c
OBJECTS_TCP_SERVER=$(SOURCES_TCP_SERVER:.c=.o)
EXEC_TCP_SERVER=socket_tcp_server

SOURCES_TCP_SERVER_2=socket_tcp_server_shm_systemv_exec.c
OBJECTS_TCP_SERVER_2=$(SOURCES_TCP_SERVER_2:.c=.o)
EXEC_TCP_SERVER_2=socket_tcp_server_shm_systemv_exec

SOURCES_SOCKET_HANDLE_CONNECTION=socket_handle_connection.c
OBJECTS_SOCKET_HANDLE_CONNECTION=$(SOURCES_SOCKET_HANDLE_CONNECTION:.c=.o)
EXEC_SOCKET_HANDLE_CONNECTION=socket_handle_connection



SOURCES_UDP_CLIENT=socket_udp_client.c
OBJECTS_UDP_CLIENT=$(SOURCES_UDP_CLIENT:.c=.o)
EXEC_UDP_CLIENT=socket_udp_client

SOURCES_UDP_SERVER=socket_udp_server.c
OBJECTS_UDP_SERVER=$(SOURCES_UDP_SERVER:.c=.o)
EXEC_UDP_SERVER=socket_udp_server




BINARIES=$(EXEC_TCP_CLIENT) $(EXEC_TCP_SERVER) $(EXEC_UDP_CLIENT) $(EXEC_UDP_SERVER) $(EXEC_TCP_SERVER_2) $(EXEC_SOCKET_HANDLE_CONNECTION)

all: $(SOURCES_TCP_CLIENT) $(SOURCES_TCP_SERVER) $(SOURCES_UDP_CLIENT) $(SOURCES_UDP_SERVER) $(BINARIES)



$(EXEC_TCP_CLIENT): $(OBJECTS_TCP_CLIENT)
	$(CC) $(LDFLAGS) $(OBJECTS_TCP_CLIENT) -o $@

$(EXEC_TCP_SERVER): $(OBJECTS_TCP_SERVER)
	$(CC) $(LDFLAGS) $(OBJECTS_TCP_SERVER) -o $@

$(EXEC_TCP_SERVER_2): $(OBJECTS_TCP_SERVER_2)
	$(CC) $(LDFLAGS) $(OBJECTS_TCP_SERVER_2) -o $@

$(EXEC_SOCKET_HANDLE_CONNECTION): $(OBJECTS_SOCKET_HANDLE_CONNECTION)
	$(CC) $(LDFLAGS) $(OBJECTS_SOCKET_HANDLE_CONNECTION) -o $@

$(EXEC_UDP_CLIENT): $(OBJECTS_UDP_CLIENT)
	$(CC) $(LDFLAGS) $(OBJECTS_UDP_CLIENT) -o $@

$(EXEC_UDP_SERVER): $(OBJECTS_UDP_SERVER)
	$(CC) $(LDFLAGS) $(OBJECTS_UDP_SERVER) -o $@



.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm *~ $(BINARIES) *.o
	

