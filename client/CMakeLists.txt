# create client executable
add_executable( client main.cpp )
target_link_libraries( client
        gpb_lib
        main_lib
        ${ZMQ_LIBRARIES} )
