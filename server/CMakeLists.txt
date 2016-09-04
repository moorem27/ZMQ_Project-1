# create error executable
add_executable( server main.cpp )
target_link_libraries( server
        gpb_lib
        main_lib
        ${ZMQ_LIBRARIES} )
