#include <chrono>
#include <string>
#include <iostream>
#include <zmq.hpp>

#include "protobuf/messages.pb.h"
#include "utilities.h"

int main () {
    using namespace util;
    using namespace std::chrono;

    const auto start = system_clock::now();

    zmq::context_t context{ 1 };

    zmq::socket_t subscriber_socket{ context, ZMQ_SUB };
    subscriber_socket.connect( "tcp://localhost:5555" );
    subscriber_socket.setsockopt( ZMQ_SUBSCRIBE, "", 0 ); // receive every message

    std::cout << "Starting Subscriber..." << std::endl;

    for ( std::size_t i = 0; i < MAX_MESSAGES; ++i ) {
        zmq::message_t received{};
        subscriber_socket.recv( &received );

        const gpb::Test_Message unpacked_message = unpack<gpb::Test_Message>( received );
        std::cout << "received " << unpacked_message << std::endl;
    }

    const auto end = system_clock::now();

    const std::locale loc{ "" };
    std::cout.imbue(loc);

    std::cout << "Subscriber done!" << std::endl;
    std::cout << "sent = " << MAX_MESSAGES << " messages" << std::endl;
    std::cout << "time = " << duration_cast<milliseconds>( end - start ).count() << " ms" << std::endl;

    return 0;
}
