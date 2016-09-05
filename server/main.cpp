#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <zmq.hpp>

void respond_to_handshake() {
    zmq::context_t context( 1 );
    zmq::socket_t zmq_socket( context, ZMQ_REP );
    zmq_socket.bind( "tcp://*:5555" );

    zmq::message_t request{};

    // Wait to receive handshake request from client
    zmq_socket.recv( &request );
    std::cout << "Server received handshake request" << std::endl;

    std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );

    zmq::message_t reply{};
    zmq_socket.send( reply );
}

//TODO: Begin breaking this apart into functions
int main( void ) {

    respond_to_handshake();

    // ----------------------- For UDP socket -------------------------
    int socket_file_descriptor = 0;
    ssize_t bytes = 0;
    socklen_t address_length = 0;
    char buffer[ 1000 ];

    // Structure containing internet address
    struct sockaddr_in server;

    // Set socket_file_descriptor to value in file descriptor table
    socket_file_descriptor = socket( AF_INET, SOCK_DGRAM, 0 );

    server.sin_family = AF_INET;            // Symbolic constant AF_INET = any two hosts on internet
    server.sin_port = htons( 4950 );        // Use htons to convert from host byte order to network byte order
    server.sin_addr.s_addr = INADDR_ANY;    // Symbolic constant INADDR_ANY = server IP address

    // Bind socket to address
    if( bind( socket_file_descriptor, (struct sockaddr*)&server, sizeof( server ) ) < 0 ) {
        std::cerr << "Did not bind successfully" << std::endl;
    }
    // ----------------------------------------------------------------

    std::cout << "Server is waiting" << std::endl;

    address_length = sizeof( server );
    int count = 0;
    while( ++count < 1000 ) {
        bytes = recvfrom( socket_file_descriptor, buffer,
                          sizeof( buffer ), 0, (struct sockaddr*)&server,
                          &address_length );
        if( bytes < 0 ) {
            std::cerr << "Receive failed" << std::endl;
        }
        buffer[ bytes ] = '\0';
        std::cout << count << " " << buffer << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    }

    close( socket_file_descriptor) ;

    return 0;
}
