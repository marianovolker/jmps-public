/*
 * socket_udp_client.c
 *
 * listens on an UDP port, accept one message and displays its content and
 * who's the sender
 *
 *
 * Created by Mij <mij@bitchx.it> on 18/12/05.
 * Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 */

#include <stdio.h>
/* socket(), bind(), recvfrom() */
#include <sys/types.h>
#include <sys/socket.h>
/* sockaddr_in */
#include <netinet/in.h>
/* inet_addr() */
#include <arpa/inet.h>
/* memset() */
#include <string.h>
/* close() */
#include <unistd.h>
/* exit() */
#include <stdlib.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    /* Variable and structure definitions. */
    int sd, rc;
    struct sockaddr_in serveraddr;
    int serveraddrlen = sizeof(serveraddr);
    char buffer[100];
    char *bufptr = buffer;
    int buflen = sizeof(buffer);
    struct hostent *hostp;


    if(argc != 3) 
    {
        /*Use default hostname or IP*/
        printf("UDP Client - Usage <Server hostname or IP>\n");
        exit(0);
    }


    memset(buffer, 0x00, sizeof(buffer));
    strcpy(buffer, "Hello! A client request message lol!");

    if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("UDP Client - socket() error");
        exit(-1);
    }
    else
        printf("UDP Client - socket() is OK!\n");

    memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[2]));

    hostp = gethostbyname(argv[1]);
    if(hostp == (struct hostent *)NULL)
    {
        printf("HOST NOT FOUND --> ");
        printf("h_errno = %d\n", h_errno);
        exit(-1);
    }
    else
    {
        printf("UDP Client - gethostname() of the server is OK... \n");
        printf("Connected to UDP server\n");
    }
    memcpy(&serveraddr.sin_addr, hostp->h_addr, sizeof(serveraddr.sin_addr));

    rc = sendto(sd, bufptr, buflen, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(rc < 0)
    {
        perror("UDP Client - sendto() error");
        close(sd);
        exit(-1);
    }
    else
        printf("UDP Client - sendto() is OK!\n");

    printf("Waiting a reply from UDP server...\n");

    rc = recvfrom(sd, bufptr, buflen, 0, (struct sockaddr *)&serveraddr, ((socklen_t *)&serveraddrlen));

    if(rc < 0)
    {
        perror("UDP Client - recvfrom() error");
        close(sd);
        exit(-1);
    }
    else
    {
        printf("UDP client received the following: \"%s\" message\n", bufptr);
    }

    close(sd);
    exit(0);
}

