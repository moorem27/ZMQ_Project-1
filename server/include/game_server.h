class Game_Server {
    public: 
        Game_Server();
        Game_Server( const Game_Server& copy );
        Game_Server& operator=(const Game_Server& copy );
        ~Game_Server();
        
    private:
        void respond_to_handshake();
        void event_loop();

        zmq::context_t handshake_context_;
        zmq::socket_t handshake_socket_;
        
        zmq::context_t client_1_context_;
        zmq::socket_t  client_1_socket_;

        zmq::context_t client_2_context_;
        zmq::context_t client_2_socket_;
}
