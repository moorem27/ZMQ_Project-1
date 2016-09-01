#include <zmq.hpp>
#include <array>
#include <iostream>
#include "utilities.h"

int main () {
    //  Prepare our context and sockets
    zmq::context_t context(1);
    zmq::socket_t frontend (context, ZMQ_ROUTER); // for clients
    zmq::socket_t backend (context, ZMQ_DEALER); // for workers

    frontend.bind("tcp://*:5559");
    backend.bind("tcp://*:5560");

    std::cout << "Broker starting..." << std::endl;

    zmq::proxy( static_cast<void*>( frontend ), static_cast<void*>( backend ), nullptr );

    std::cout << "Broker done!" << std::endl;

    return 0;
}