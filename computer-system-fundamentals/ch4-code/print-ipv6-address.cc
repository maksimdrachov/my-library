/*  Utility program that prints IPv6 addresses as a readable string */

#include <netdb.h>  /* struct addrinfo */
#include <stdio.h>  /* NULL */
#include <stdlib.h> /* NULL */
#include <cstring>  /* memset */

#define HOSTNAME "google.com"

int main(){
    struct addrinfo hints;
    void *server_list = NULL;
    void *server = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;         /* change to AF_INET for IPv4 */
    hints.ai_socktype = SOCK_STREAM;    /* limit to byte-streams, TCP transport; change to SOCK_DGRAM for UDP transport */
    hints.ai_protocol = IPPROTO_TCP;    /* create as a TCP socket */

    /* Get a list of addresses at hostname that serve HTTP */
    getaddrinfo(HOSTNAME, "http", &hints, &server_list);

}

/*  Code Listing 4.6
    Client code that will connect to a web server for an HTTP session */

/* Declare an IPv4 socket for TCP */
int socketfd = -1;
struct addrinfo hints;
struct addrinfo *server_list = NULL;
struct addrinfo *server = NULL;

memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;

/* Get a list of addresses at hostname that serve HTTP */
getaddrinfo(hostname, "http", &hints, &server_list);

/*  Code Listning
    Setting up a connection-oriented server and receivin connections */
int socket_option = 1;
for (server = server_info; server != NULL; server = server->ai_next)
{
    /* Attempt to create a TCP socket */
    if ((socketfd = socket(server->ai_family, server->ai_socktype, 0)) < 0)
        continue
    
    /* Configure the socket to ignore bind reuse error */
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &socket_option,
                sizeof (int));
    /* Set a 5-second timeout when waiting to receive */
    struct timeval timeout = {5, 0};
    setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (const void *) &timeout,
                sizeof(timeout));
            
    /* Bind the TCP socket to the port number */
    if (bind(socketfd, server->ai_addr, server->ai_addrlen) == 0)
        break;
    close(socketfd);
    socketfd = -1;   
}

/*  Code Listning
    Getting socket address information for a server */

struct addrinfo hints, *server_info = NULL;
memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_INET;          /* grab IPv4 only */
hints.ai_socktype = SOCK_STREAM;    /* specify byte-streaming */
hints.ai_flags = AI_PASSIVE;        /* use default IP address */
hints.ai_protocol = IPPROTO_TCP;    /* create as a TCP socket */

/* Get a list of addresses at hostname that serve HTTP */
getaddrinfo(NULL, "8000", &hints, &server_info);

/*  Code Listning
    Setting up a connection-oriented server and receiving connections
    Demonstrates using setsockopt() and bind() to extend the prev code listing
    to set up a TCP server */
int socket_option = 1;

/*  Code Listning
    Receiving TCP connections
    Shows the final steps of setting up a TCP connection-oriented server
    and receiving requests, beginning with the call to listen() */

/* Convert to server socket */
listen(socketfd, 10);

/* Get the size of the sockaddr from getaddrinfo() results */
socklen_t addrlen = server->ai_addrlen;

while (1) {
    /* Allocate space for the incoming address info and get it */
    struct sockaddr *address = calloc(1, (size_t)addrlen);
    assert (address != NULL);

    int connection;
    if ((connection = accept(socketfd, address, &addrlen)) < 0)
        break;
    
    /* Print information about the connection and close it */
    struct sockaddr_in *addr = (struct sockaddr_in *)server->ai_addr;
    printf("Incoming request from %s:%" PRI16D "\n", inet_ntoa(addr->sin_addr), 
                ntohs(addr->sin_port));
    
    close(connection);
    free(address);
}
close(socketfd);

/*  Code Listing
    Sending to and receiving from an IPv6 socket
    illustrates sending a simple HTTP request and reading the first part of the response */

/* Create a message for a simple HTTP request */
size_t buffer_len = 100;
char buffer[buffer_len];
memset(buffer, 0, buffer_len);
strncpy(buffer, "GET / HTTP/1.0\r\n\r\n", buffer_len);

/* When sending, you can check the number of bytes sent */
ssize_t bytes = sendto(socketfd, buffer, buffer_len, 0,
                        server->ai_addr, server->ai_addrlen);
                
/* Copy the server IP address into a buffer */
char addr_buffer[INET6_ADDRSTRLEN];
/* inet_ntop - convert IPv4 and IPv6 addresses from binary to text form */
inet_ntop(AF_INET6, &((struct sockaddr_in6 *)server->ai_addr)->sin6_addr,
            addr_buffer, sizeof(addr_buffer));
/* %zd format expects an argument of the signed type that corresponds to the unsigned type size_t */
printf("Sent %zd bytes to %s\n", bytes, addr_buffer);

/* Read all data into the buffer, keeping space for \0 at end */
while ((bytes = recvfrom(socketfd, buffer, buffer_len-1, 0, server->ai_addr,
                            &server->ai_addrlen)) > 0)
    {
        printf("%s", buffer);
        memset(buffer, 0, buffer_len);
    }
close(socketfd);