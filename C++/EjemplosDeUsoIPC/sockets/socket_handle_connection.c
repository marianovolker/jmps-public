/*
 * socket_tcp_server_shm_systemv_exec.c
 *
   Handle Multiple Connections:
   To allow the server to handle multiple simultaneous connections, we make the following changes in the above code:

   Put the accept statement and the following code in an infinite loop.

   After a connection is established, call fork() to create a new process.

   The child process will close sockfd and call doprocessing function, passing the new socket file descriptor as an argument. When the two
   processes have completed their conversation, as indicated by doprocessing() returning, this process simply exits.

   The parent process closes newsockfd. Because all of this code is in an infinite loop, it will return to the accept statement to wait for 
   the next connection.
 *
 *
 * Created by Mij <mij@bitchx.it> on 18/12/05.
 * Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 */

#include <stdio.h>
/* socket(), bind() */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
/* write(), close() */
#include <unistd.h>
/* strlen() */
#include <string.h>
/* exit() */
#include <stdlib.h>
#include <netinet/in.h>

#include <sys/ipc.h>
#include <sys/shm.h>


#define SHMSZ     sizeof(int)

int get_socket_connection()
{
    int shmid;
    key_t key;
    int *shm;

    /*
     * We need to get the segment named
     * "0x5678", created by the server.
     */
    key = 0x5678;

    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) 
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = (int*)shmat(shmid, NULL, 0)) == (void *) -1)
    {
        perror("shmat");
        exit(1);
    }

    int socket_fd = (*shm);

    printf("socket fd after exec shmat: %d\n",socket_fd);

    return socket_fd;
}

void doprocessing (int sock)
{
    int n;
    char buffer[256];

    bzero(buffer,256);

    printf("socket fd after exec: %d\n",sock);

    n = read(sock,buffer,255);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    printf("Here is the message: %s\n",buffer);
    n = write(sock,"I got your message",18);
    if (n < 0) 
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}

int main( int argc, char *argv[] )
{
    int sockfd = get_socket_connection();

    doprocessing(sockfd);

    exit(0);
}



