/*
 * socket_tcp_client.c
 *
   We have seen in one of the tutorials that to make a process a TCP client following steps are required:  
   Create a socket with the socket() system call.
   Connect the socket to the address of the server using the connect() system call.
   Send and receive data. There are a number of ways to do this, but the simplest is to use the read() and write() system calls.
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
#include <netdb.h>


int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (argc < 3) 
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect( sockfd, ((struct sockaddr *)&serv_addr), sizeof(serv_addr) ) < 0) 
    {
         perror("ERROR connecting");
         exit(1);
    }

    /* Now ask for a message from the user, this message
    * will be read by server
    */
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    /* Send message to the server */
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
    {
         perror("ERROR writing to socket");
         exit(1);
    }

    /* Now read server response */
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
    {
         perror("ERROR reading from socket");
         exit(1);
    }
    printf("%s\n",buffer);
    return 0;
}


