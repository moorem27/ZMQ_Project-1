#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <thread>
#include <arpa/inet.h>


int main( void )
{
    int socket_file_descriptor = 0;

    // Structure containing internet address
    struct sockaddr_in client;

    socket_file_descriptor = socket( AF_INET, SOCK_DGRAM, 0 );
    memset( &client, 0, sizeof( client ) );

    client.sin_family = AF_INET;
    client.sin_port = htons( 5555 );
    client.sin_addr.s_addr = inet_addr( "127.0.0.1" );

    int count = 0;
    while( ++count < 1000 ) {
        char *buffer = (char *) "You're right, Sean Murray, it isn't any man's sky.";
        if( sendto( socket_file_descriptor, buffer, strlen( buffer ), 0,
                    (struct sockaddr *)&client, sizeof( client ) ) == -1 ) {
            perror( "Send failed" );
            exit( 1 );
        }
        std::cout << count << " Client sent: " << buffer << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    }
    close( socket_file_descriptor );

    return 0;
}
