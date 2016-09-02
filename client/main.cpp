// @file client/main.cpp
// @brief A datagram client demo



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <thread>
#include <chrono>

#define SERVERPORT "4950"    // the port users will be connecting to

int main( void )
{
    int socket_file_descriptor = 0;
    struct addrinfo client, *servinfo, *p;
    int rv;

    memset( &client, 0, sizeof( client ) );
    client.ai_family = AF_UNSPEC;
    client.ai_socktype = SOCK_DGRAM;
    client.ai_flags = AI_PASSIVE;

    if ( ( rv = getaddrinfo( NULL, SERVERPORT, &client, &servinfo ) ) != 0 ) {
        fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( rv ) );
        return 1;
    }

    // loop through all the results and make a socket
    for( p = servinfo; p != NULL; p = p->ai_next ) {
        if ( ( socket_file_descriptor = socket( p->ai_family, p->ai_socktype,
                             p->ai_protocol) ) == -1) {
            perror( "talker: socket" );
            continue;
        }

        break;
    }

    if ( p == NULL ) {
        fprintf( stderr, "talker: failed to create socket\n" );
        return 2;
    }

    int count = 0;
    while( ++count < 1000 ) {
        char *buffer = (char *) "You're right, Sean Murray, it isn't any man's sky.";
        if ( ( sendto( socket_file_descriptor, buffer, strlen( buffer ), 0,
                    p->ai_addr, p->ai_addrlen ) ) == -1) {
            perror( "talker: sendto" );
            exit( 1 );
        }
//        free( buffer );
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    }
    freeaddrinfo( servinfo );
    close( socket_file_descriptor );

    return 0;
}