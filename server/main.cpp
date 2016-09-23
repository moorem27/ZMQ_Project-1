#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <zmq.hpp>

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


int main( void ) {

    respond_to_handshake();

    return 0;
}
