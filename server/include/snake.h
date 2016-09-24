class Snake {
public:
    Snake();
    Snake( const Snake &snake );
    Snake &operator=( const Snake &copy );
    ~Snake();

    void set_x( const int x ) { x_coordinate_ = x; }
    void set_y( const int y ) { y_coordinate_ = y; }

    int get_x() { return x_coordinate_; }
    int get_y() { return y_coordinate_; }

    unsigned long get_hash() { return hash_; }
    void set_hash( const char* client_name );
private:
    inline unsigned long hash( const char* client_name ) {
        int hash_value = 0;
        while ( *client_name )
            hash_value = hash_value << 1 ^ *client_name++;
        return hash_value;
    }

    long hash_;
    int x_coordinate_;
    int y_coordinate_;
};
