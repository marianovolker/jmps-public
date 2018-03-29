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

void put_socket_connection(int socket)
{
    int shmid;
    key_t key;
    int *shm;

    /*
     * We'll name our shared memory segment
     * "0x5678".
     */
    key = 0x5678;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = (int *)shmat(shmid, NULL, 0)) == (void *) -1)
    {
        perror("shmat");
        exit(1);
    }

    *shm=socket;

    printf("socket fd after shmat: %d\n", (*shm) );
}

void handle_connection ()
{
    if( execl("socket_handle_connection", "socket_handle_connection", NULL, NULL, NULL) < 0 )
    {
       fprintf(stderr, "Problems execution socket_handle_connection binary.\n");
       exit(1);
    }
}

int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
 
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }
    /* Now start listening for the clients, here 
     * process will go in sleep mode and will wait 
     * for the incoming connection
     */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while (1) 
    {
        newsockfd = accept(sockfd, ((struct sockaddr *) &cli_addr), (socklen_t *)&clilen);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }

        /* Create child process */
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("ERROR on fork");
	    exit(1);
        }

        if (pid == 0)  
        {
            printf("socket fd: %d\n",newsockfd);

            /* This is the client process */
            put_socket_connection(newsockfd);
            handle_connection();
            exit(0);
        }
        else
        {
            close(newsockfd);
        }
    } /* end of while */
}



