// @file server/main.cpp
// @brief A datagram server demo

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <thread>
#include <chrono>

#define MYPORT "4950"    // the port users will be connecting to

#define MAXBUFLEN 100

// get sockaddr, IPv4 or IPv6:
void *get_in_addr( struct sockaddr *sa )
{
    if ( sa->sa_family == AF_INET ) {
        return &( ( ( struct sockaddr_in*) sa )->sin_addr ); //TODO: What the hell is this?
    }

    return &( ( ( struct sockaddr_in6* )sa )->sin6_addr );   //TODO: " "?
}

int main( void ) {
    int socket_file_descriptor = 0;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    ssize_t bytes;
    struct sockaddr_storage their_addr;
    char buffer[ MAXBUFLEN ];
    socklen_t addr_len;
    char s[ INET6_ADDRSTRLEN ];

    memset( &hints, 0, sizeof hints );
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ( ( rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo ) ) != 0 ) {
        fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( rv ) );
        return 1;
    }

    // loop through all the results and bind to the first we can
    for ( p = servinfo; p != NULL; p = p->ai_next ) {
        if ( ( socket_file_descriptor = socket( p->ai_family, p->ai_socktype,
                             p->ai_protocol ) ) == -1 ) {
            perror( "listener: socket" );
            continue;
        }

        if ( bind( socket_file_descriptor, p->ai_addr, p->ai_addrlen ) == -1 ) {
            close( socket_file_descriptor );
            perror( "listener: bind" );
            continue;
        }

        break;
    }

    if ( p == NULL ) {
        fprintf( stderr, "listener: failed to bind socket\n" );
        return 2;
    }

    freeaddrinfo( servinfo );

    printf( "listener: waiting to recvfrom...\n" );

    addr_len = sizeof their_addr;
    int count = 0;
    while( ++count < 1000 ) {
        if ( ( bytes = recvfrom( socket_file_descriptor, buffer, MAXBUFLEN - 1, 0,
                              (struct sockaddr *) &their_addr, &addr_len ) ) == -1 ) {
            perror("recvfrom");
            exit(1);
        }
        std::cout << "listener: got packet from " << inet_ntop( their_addr.ss_family,
                                                               get_in_addr( ( struct sockaddr * ) &their_addr ),
                                                               s, sizeof s ) << std::endl;
        std::cout << "listener: packet is " << bytes << " bytes long" << std::endl;
        buffer[ bytes ] = '\0';
        std::cout << count << " " << buffer << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
        std::cout << "\n" << std::endl;
    }

    close( socket_file_descriptor) ;

    return 0;
}

