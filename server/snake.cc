#include "snake.h"

Snake::Snake() {
    set_x( 0 );
    set_y( 50 ); // TODO set_y( screen_height / 2 );

    // TODO: Set X and Y coordinates based on hash
    // if( matts_hash )
    //     x = something
    // else
    //     x = something_else
}

Snake::~Snake() {
}

void Snake::set_hash( const char* client_name ) {
    hash_ = hash( client_name );
}
