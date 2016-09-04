add_executable( client_example main.cpp )
target_link_libraries( client_example
        gpb_lib
        main_lib
${ZMQ_LIBRARIES} )
