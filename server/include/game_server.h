#include <zmq.hpp>
#include "snake.h"

class Game_Server {
    public: 
        Game_Server();
        Game_Server( const Game_Server& copy );
        Game_Server& operator=(const Game_Server& copy );
        ~Game_Server();
        
    private:
        void respond_to_handshake();
        void event_loop();
        void initialize(); // make public?
        void update_coordinates( const int x, const int y );
        bool detect_collision( Snake& snake_1, Snake& snake_2 );

        zmq::context_t  handshake_context_;
        zmq::socket_t   handshake_socket_;
        
        zmq::context_t  pull_context_;
        zmq::socket_t   pull_socket_;

        zmq::context_t  publish_context_;
        zmq::socket_t   publish_socket_;

        Snake snake_1_;
        Snake snake_2_;

        bool            is_running_;
};
