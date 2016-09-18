#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <zmq.hpp>

//TODO: Clean up this code, it's nasty

constexpr int UDP_PORT_NUMBER   = 4950;
constexpr int PI_TO_ARD_PORT	= 5005;
constexpr int BUFFER_LIMIT      = 65507;
const     std::string SHUTDOWN  = "shutdown";

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


void initialize_udp_socket( int& socket_file_descriptor, sockaddr_in& server ) {
    socket_file_descriptor = socket( AF_INET, SOCK_DGRAM, 0 );

    server.sin_family      = AF_INET;                  // Symbolic constant AF_INET = any two hosts on internet
    server.sin_port        = htons( UDP_PORT_NUMBER ); // Convert from host byte order to network byte order
    server.sin_addr.s_addr = INADDR_ANY;               // Symbolic constant INADDR_ANY = server IP address

    // Bind socket to address
    if( bind( socket_file_descriptor, (struct sockaddr*)&server, sizeof( server ) ) < 0 ) {
        std::cerr << "Did not bind successfully" << std::endl;
    }
}


void receive_messages( const int socket_file_descriptor, sockaddr_in& server ) {
    ssize_t bytes                   = 0;
    socklen_t address_length        = 0;
    bool is_running                 = true;
    char buffer[ BUFFER_LIMIT ];
    int send_descriptor 	    = socket( AF_INET, SOCK_DGRAM, 0 );
    struct sockaddr_in client{};
    std::string send_message = " ";
    size_t message_size = strlen( send_message.c_str() );

    client.sin_family = AF_INET;
    client.sin_port = htons( 5005 );
    client.sin_addr.s_addr = inet_addr( "your.server.ip.address" );

    std::cout << "Server is waiting" << std::endl;

    address_length  = sizeof( server );

    while( is_running ) {
        bytes = recvfrom( socket_file_descriptor, buffer,
                          sizeof( buffer ), 0, (struct sockaddr*)&server,
                          &address_length );
        if( bytes < 0 ) {
            std::cerr << "Receive failed" << std::endl;
        } else {
            std::cout << "Forwarding to Arduino " << std::string{ buffer } << std::endl;
            sendto( send_descriptor, send_message.c_str(), message_size, 0, (struct sockaddr*)&client, sizeof( client) ); 
	}
        if( std::string{ buffer } == SHUTDOWN )
            is_running = false;
        memset( &buffer, 0, BUFFER_LIMIT );
    }

}


int main( void ) {
    int socket_file_descriptor = 0;
    struct sockaddr_in server{};

    initialize_udp_socket( socket_file_descriptor, server );
    respond_to_handshake();
    receive_messages( socket_file_descriptor, server );

    close( socket_file_descriptor) ;
    return 0;
}
