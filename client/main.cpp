#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <zmq.hpp>

constexpr int MAX_MESSAGE_SIZE  = 65507;

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


void initialize_udp_socket( int& socket_file_descriptor, sockaddr_in& client ) {
    socket_file_descriptor  = socket( AF_INET, SOCK_DGRAM, 0 );
    memset( &client, 0, sizeof( client ) );

    client.sin_family       = AF_INET;
    client.sin_port         = htons( 4950 );
    client.sin_addr.s_addr  = inet_addr( "127.0.0.1" );
}


void send_messages( const int socket_file_descriptor, const sockaddr_in& client, std::string message ) {
    size_t message_size = strlen( message.c_str() );

    if( message_size < MAX_MESSAGE_SIZE ) {
        if ( sendto( socket_file_descriptor, message.c_str(), message_size, 0,
                     (struct sockaddr *) &client, sizeof( client ) ) == -1 ) {
            perror( "Send failed" );
            exit( 1 );
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    } else {
        std::cerr << "Message size exceeds limit" << std::endl;
    }
}


int main( void ) {
    int socket_file_descriptor = 0;
    struct sockaddr_in client{};
    std::string message{};

    bool handshake_complete = complete_handshake();

    if( handshake_complete ) {
        initialize_udp_socket( socket_file_descriptor, client );

        // TODO: Add condition to while loop
        while( 1 ) {
            // Block for message, signal, etc
            std::cout << "Enter a message: ";
            std::getline( std::cin, message );

            send_messages( socket_file_descriptor, client, message );
        }
    }

    close( socket_file_descriptor );
    return 0;
}
