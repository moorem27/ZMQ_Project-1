#include <chrono>
#include <iostream>
#include <thread>
#include <zmq.hpp>

#include <protobuf/messages.pb.h>
#include "utilities.h"

int main() {
    using namespace util;

    zmq::context_t context(1);

    zmq::socket_t responder(context, ZMQ_REP);
    responder.connect("tcp://localhost:5560");

    int32_t reply_count = 0;

    std::cout << "Worker starting..." << std::endl;

    const std::string NUMBER_REQUEST_NAME = gpb::Number_Request{}.GetTypeName();

    while( true ) {
        zmq::message_t request_message{};
        responder.recv( &request_message );

        gpb::Message top_level_message = unpack_to_top_level( request_message );

        if ( NUMBER_REQUEST_NAME == top_level_message.name() ) {
            std::cout << "Worker received number request" << std::endl;

//            std::this_thread::sleep_for( SLEEP_TIME ); // do some work

            gpb::Number_Reply number_reply{};
            number_reply.set_num( reply_count );

            zmq::message_t reply_message = pack( number_reply );
            responder.send( reply_message );

            std::cout << "Worker sent: " << number_reply.num() << std::endl;

            ++reply_count;
        }
        else {
            std::cout << "!!! Worker received unknown message type !!!" << std::endl;
            std::cout << "\treceived = " << top_level_message.name() << " !!!" << std::endl;
        }

    }

    return 0;
}
