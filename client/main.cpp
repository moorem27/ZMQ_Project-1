#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <zmq.hpp>

bool complete_handshake() {
    bool handshake_complete = false;
    // ----------------- For ZMQ socket -------------------------
    zmq::context_t context( 1 );
    zmq::socket_t zmq_socket( context, ZMQ_REQ );

    std::cout << "Connecting to server " << std::endl;
    zmq_socket.connect( "tcp://127.0.0.1:5555" );

    // Send handshake request to server
    zmq::message_t request{};
    zmq_socket.send( request );

    // Wait on handshake reply before proceeding
    zmq::message_t reply{};
    if( zmq_socket.recv( &reply ) ) {
        std::cout << "Client received handshake reply" << std::endl;
        handshake_complete = true;
    }

    return handshake_complete;
}

//TODO: Begin breaking this apart into functions
int main( void )
{
    bool handshake_complete = complete_handshake();

    // ------------------ For UDP socket -----------------------
    int socket_file_descriptor = 0;

    // Structure containing internet address
    struct sockaddr_in client;

    socket_file_descriptor = socket( AF_INET, SOCK_DGRAM, 0 );
    memset( &client, 0, sizeof( client ) );

    client.sin_family = AF_INET;
    client.sin_port = htons( 4950 );
    client.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    // ----------------------------------------------------------

    if( handshake_complete ) {
        std::cout << "Handshake complete" << std::endl;
        int count = 0;
        while ( ++count < 100 ) {
            std::string message = "All types of pizza are welcome.";

            if ( sendto( socket_file_descriptor, message.c_str(), sizeof( message ), 0,
                       (struct sockaddr *) &client, sizeof( client ) ) == -1) {
                perror( "Send failed" );
                exit( 1 );
            }
            std::cout << count << " Client sent: " << message << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
       }
        close( socket_file_descriptor );
    }


    return 0;
}
