#include "game_server.h"
#include <thread>
#include <iostream>

Game_Server::Game_Server(): handshake_context_( 1 ),
                            handshake_socket_( handshake_context_, ZMQ_REP ),
                            pull_context_( 1 ),
                            pull_socket_( pull_context_, ZMQ_PULL ),
                            publish_context_( 1 ),
                            publish_socket_( publish_context_, ZMQ_PUB ),
                            snake_1_(),
                            snake_2_(),
                            is_running_( false )
{
    initialize();
}


Game_Server::~Game_Server() {
    is_running_ = false;
}


void Game_Server::initialize() {
    respond_to_handshake();
    event_loop();
}


void Game_Server::respond_to_handshake() {
    bool shaking_hands = true;
    int number_of_clients = 0;
    zmq::message_t request{};

    handshake_socket_.bind( "tcp://*:5555" );

    // Wait to receive handshake request from both clients
    while( shaking_hands ) {
        if( handshake_socket_.recv( &request ) ){
            zmq::message_t reply{};
            handshake_socket_.send( reply );
            ++number_of_clients;
        }

        if( number_of_clients == 2 ){
            is_running_     = true;
            shaking_hands   = false;
        }
    }
}


void Game_Server::event_loop() {
    zmq::message_t buffer{};

    while( is_running_ ) {
	if( pull_socket_.recv( &buffer ) ) {
            // TODO:
            //     Unwrap message to receive contents
            //     update_snake_coordinates
	        //     detect_collision
            //     publish_socket_.send( contents );
        }
    }
}


bool Game_Server::detect_collision( Snake &snake_1, Snake &snake_2 ) {
    //TODO
}











