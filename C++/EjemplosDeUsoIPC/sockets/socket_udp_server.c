/*
 * socket_udp_server.c
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


int main(int argc, char *argv[])
{
    /* Variable and structure definitions. */
    int sd, rc;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t clientaddrlen = sizeof(clientaddr);
    int serveraddrlen = sizeof(serveraddr);
    char buffer[100];
    char *bufptr = buffer;
    int buflen = sizeof(buffer);

    if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("UDP server - socket() error");
        exit(-1);
    }

    printf("UDP server - socket() is OK\n");

    memset(&serveraddr, 0x00, serveraddrlen);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(0);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if((rc = bind(sd, (struct sockaddr *)&serveraddr, serveraddrlen)) < 0) 
    {
        perror("UDP server - bind() error");
        close(sd);
        exit(-1);
    }

    int addr_len = sizeof(serveraddr);
    if (getsockname(sd, (struct sockaddr *) &serveraddr, ((socklen_t *)&addr_len))<0) 
    {
        perror("Error getting socket name.\n");
        return -1;
    }
    printf("Using IP %s and port %d\n", inet_ntoa(serveraddr.sin_addr), ntohs(serveraddr.sin_port));
    printf("UDP server - Listening...\n");

    rc = recvfrom(sd, bufptr, buflen, 0, (struct sockaddr *)&clientaddr, ((socklen_t *)&clientaddrlen));
    if(rc < 0) 
    {
        perror("UDP Server - recvfrom() error");
        close(sd);
        exit(-1);
    }

   printf("UDP Server received the following:\n \"%s\" message\n", bufptr);

   printf("UDP Server replying to the UDP client...\n");
   rc = sendto(sd, bufptr, buflen, 0, (struct sockaddr *)&clientaddr, clientaddrlen);
   if(rc < 0) 
   {
       perror("UDP server - sendto() error");
       close(sd);
       exit(-1);
   }
   printf("UDP Server - sendto() is OK...\n");

   close(sd);
   exit(0);
}
