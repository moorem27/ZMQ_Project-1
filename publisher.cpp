#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#include <zmq.hpp>

#include "protobuf/messages.pb.h"
#include "utilities.h"


int main( void ) {
    using namespace util;
    using namespace std::chrono;

    const auto start = system_clock::now();

    zmq::context_t context{ 1 };
    zmq::socket_t  publish_socket{ context, ZMQ_PUB };
    publish_socket.bind( "tcp://*:5555" );

    std::cout << "Starting publisher..." << std::endl;

    std::this_thread::sleep_for( std::chrono::milliseconds{ 500 } ); // prevents "slow joiner"

    for ( std::size_t  i = 0; i < MAX_MESSAGES; ++i ) {
        std::ostringstream os;
        os << "message " << i;

        const gpb::Test_Message gpb_message = test_message(static_cast<uint32_t >( i ), os.str() );

        publish_socket.send( pack<>( gpb_message ) );

        std::cout << "sent: " << " " << gpb_message << std::endl;
    }

    const auto end = system_clock::now();

    const std::locale loc{ "" };
    std::cout.imbue(loc);

    std::cout << "Publisher done!" << std::endl;
    std::cout << "sent = " << MAX_MESSAGES << " messages" << std::endl;
    std::cout << "time = " << duration_cast<milliseconds>( end - start ).count() << " ms" << std::endl;


    return 0;
}