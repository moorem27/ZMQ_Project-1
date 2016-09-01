#include <thread>
#include <chrono>
#include <zmq.hpp>
#include <sstream>
#include "utilities.h"

int main () {
    using namespace util;

    zmq::context_t context(1);

    zmq::socket_t requester(context, ZMQ_REQ);
    requester.connect("tcp://localhost:5559");

    std::cout << "Client starting..." << std::endl;

    const std::string NUMBER_REPLY_NAME = gpb::Number_Reply{}.GetTypeName();

    while ( true ) {
        zmq::message_t message = pack( gpb::Number_Request{} );
        requester.send( message ); // send request

        std::cout << "Client sent number request..." << std::endl;

        zmq::message_t rcv_message{};
        requester.recv( &rcv_message ); // receive reply

        const gpb::Message top_level_message = unpack_to_top_level( rcv_message );

        if ( NUMBER_REPLY_NAME == top_level_message.name() ) {
            const auto unpacked_message = unpack<gpb::Number_Reply>( rcv_message );
            std::cout << "Client received: " << unpacked_message.num() << std::endl;
        }
        else {
            std::cout << "!!! Client received unknown message type !!!" << std::endl;
            std::cout << "\treceived name = " << top_level_message.name() << std::endl;
        }

        std::this_thread::sleep_for( SLEEP_TIME );
    }

    return 0;
}